// LS2Type_Foo.cpp - example from a LavishScript 2.0 module

// include LavishScript 2.0 Module stuff
#include <ls2module.h>

#define g_pLS2Environment LavishScript2::ILS2StandardEnvironment::s_pInstance

// set up shorthand form of the LS2 value class we'll use for our ActualFoo class
typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<class ActualFoo*> LS2CodeBoxValue_ActualFoo;

// Here's our actual foo class. 
// This is the object we're working with.
// it's a completely custom class with no relation to LavishScript, but I added reference counting for resource management.
// using this method of reference counting we can use LS2SmartRef
class ActualFoo
{
public:
	ActualFoo()
	{
		// assume we have a reference to start out with.
		m_RefCount=1;
	}

	// with this constructor we provide a custom value to m_SomeValue. We can support both constructors with Foo::New()!
	ActualFoo(const wchar_t *initial_value):m_SomeValue(initial_value)
	{
		m_RefCount=1;
	}

	virtual ~ActualFoo()
	{
		// this class doesn't need to do anything special in its destructor.
		// for the sample, we'll printf (by the way printf is NOT being redirected)
		printf("ActualFoo::~ActualFoo()\n");
	}

	int m_RefCount; // The reference count for this object

	// add a reference count to this object
	virtual int AddRef()
	{
		m_RefCount++;
		return m_RefCount;
	}

	// decrement the reference count, deleting this object if necessary
	virtual int Delete()
	{
		m_RefCount--;
		if (m_RefCount<=0)
		{
			delete this;
			return 0;
		}
		return m_RefCount;
	}

	// given a LS2CodeBoxValue containing an ActualFoo, pull out either an ActualFoo or an LS2Exception
	static bool GetFrom(LavishScript2::LS2CodeBoxValue &subject, ActualFoo **ppFoo, LavishScript2::LS2Exception **ppException)
	{
		// validate the subject and set up pSubjectView variable
		LS2_SUBJECT_AS_OBJECT_(s_pFooType,LS2CodeBoxValue_ActualFoo,pSubjectView);

		// semi-longhand form of the above (LS2_SUBJECT_AS_OBJECT is another macro that retrieves a LavishScript2::LS2CodeBoxValue_Object, the parent class of the ref counted template version)
		
		// convert "subject" to a LS2CodeBoxValue_Object, or return false & give an exception
		/*
		LS2_SUBJECT_AS_OBJECT(objSubject);

		// now make sure it's a Foo type
		if (&objSubject->m_Type!=s_pFooType)
		{
			*ppException = new LavishScript2::LS2IllegalSubjectException("ILLEGAL SUBJECT! BURN ALL BOOKS!");
			return false;
		}
		// it's a Foo alright.

		LS2CodeBoxValue_ActualFoo *pSubjectView = (LS2CodeBoxValue_ActualFoo *)objSubject;
		/**/

		// increment our ActualFoo object's ref count (m_Object)
		pSubjectView->m_Object->AddRef();
		
		// provide the extracted ActualFoo object
		*ppFoo = pSubjectView->m_Object;
		return true;
	}

	// given an ActualFoo, retrieve a LS2CodeBoxValue containing it
	void GetLS2CodeBoxValue(LavishScript2::LS2CodeBoxValue **ppValue)
	{
		*ppValue = new LS2CodeBoxValue_ActualFoo(*s_pFooType,this);
	}

	// Some value stored in an ActualFoo. we'll use a Property to access it!
	LavishScript2::LS2String m_SomeValue;

	// a reference to the Foo type so we can instantiate objects, compare types of arbitrary objects, etc
	static LavishScript2::ILS2CodeBoxType *s_pFooType;

	// this our allocator for ActualFoo objects, in order for the Foo.Foo type to support the NEWOBJECT instruction
	class Allocator : public LavishScript2::ILS2CodeBoxValueAllocator
	{
	public:
		Allocator()
		{
			// assume we have a reference to start out with.
			m_RefCount = 1;

			// we don't need to allocate a meta table unless it's going to be used. in this case we'll assume it wont be, and defer allocation.
			m_pMetaTable = 0; 
		}
		virtual ~Allocator()
		{
			// if we allocated a meta table, make sure we free our reference
			if (m_pMetaTable)
			{
				m_pMetaTable->Delete();
				m_pMetaTable=0;
			}
		}

			int m_RefCount;

			virtual int AddRef()
			{
				m_RefCount++;
				return m_RefCount;
			}

			virtual int Delete()
			{
				m_RefCount--;
				if (m_RefCount<=0)
				{
					delete this;
					return 0;
				}
				return m_RefCount;
			}

			// this is called to retrieve the meta table for this allocator
			virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)
			{
				// we deferred allocation because we don't expect our allocator's meta table to be used
				if (!m_pMetaTable)
				{
					// in this case, someone wanted to use our meta table, maybe to store some data, so we'll initialize one
					LavishScript2::ILS2CodeBoxEnvironment::s_pInstance->NewTable(&m_pMetaTable);
				}

				m_pMetaTable->AddRef();
				*ppTable = m_pMetaTable;
				return true;
			}

			// if no constructor is passed in, inputs will not be there either. this would mean to allocate default<T>
			// if a constructor is passed in, inputs will probably be there. we would initially construct the object in C++ land, and then the LS2-land constructor will be called.
			virtual bool Allocate(LavishScript2::ILS2CodeBoxMethod *pConstructor_optional, LavishScript2::ILS2Array *pInputs_optional, LavishScript2::LS2CodeBoxValue **ppNewObject, LavishScript2::LS2Exception **ppException)
			{
				// these will come in a pair. if we're calling a constructor, the inputs are not optional.
				if (!pConstructor_optional && !pInputs_optional)
				{
					// *** NO CONSTRUCTOR AND NO INPUTS MEANS THIS IS default<T> ***
					// this is null (no object) for this kind of type, but would be 0 for an integer, etc.
					LavishScript2::ILS2CodeBoxEnvironment::s_pInstance->NewNull((LavishScript2::LS2CodeBoxValue_Null**)ppNewObject);
					//	LavishScript2::ILS2CodeBoxEnvironment::s_pInstance->NewInteger(0,(LavishScript2::LS2CodeBoxValue_Integer**)ppNewObject);
					return true;
				}

				// an LS2-land constructor was provided, and will be called after we return from Allocate() if we return true
				// so we will call the least-constructliest one we have. I like my made up word.
				ActualFoo *pActualFoo = new ActualFoo();

				// generate the LS2CodeBoxValue into ppNewObject, that points at our ActualFoo
				pActualFoo->GetLS2CodeBoxValue(ppNewObject);

				// creating the LS2CodeBoxValue_ActualFoo incremented the reference count of pActualFoo to 2 (it holds a ref). drop it back to 1 because we do not hold this reference.				
				pActualFoo->Delete();
				pActualFoo=0;// to demonstrate that we do not hold this reference
				return true;
			}
			
			LavishScript2::ILS2Table *m_pMetaTable;
	};


};

// instantiate the static reference to the Foo type
LavishScript2::ILS2CodeBoxType *ActualFoo::s_pFooType=0;


// The namespaces are from a copy/paste but shows one way of organizing things
namespace LavishScript2
{
	namespace Types
	{
		class Foo
		{
		public:

			// This is an array enumeration callback. It prints the string value of an array element.
			static bool __stdcall print_enum(unsigned int nValue, class LavishScript2::LS2CodeBoxValue &value, void *pass_thru)
			{
				// smart refs handle decrementing the ref count when leaving scope
				LavishScript2::LS2SmartRef<LavishScript2::ILS2String> pString;
				LavishScript2::LS2SmartRef<LavishScript2::LS2Exception> pException;

				// either convert the value to a string, or we should have an exception
				if (!value.GetString(pString,pException))
				{
					class LavishScript2::LS2Exception **ppException = (class LavishScript2::LS2Exception **)pass_thru;
					*ppException = pException;
					pException->AddRef();
					return false;
				}

				// print the value
				printf("%ls\n",pString->c_str());		
				return true;
			}

			// An example of a Static Method implementation
			static bool __stdcall Bar(LavishScript2::ILS2Array *pInputs, class LavishScript2::LS2Exception **ppException)
			{
				// If there are no inputs, we have nothing to print.
				if (!pInputs)
				{
					return true;
				}

				// If there are inputs, print the string value of each one
				return pInputs->Enumerate(0,print_enum,ppException);
			}

			// An example of a Property implementation
			static bool __stdcall GetSomeValue(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)
			{
				LavishScript2::LS2SmartRef<ActualFoo> pFoo;
				// retrieve an ActualFoo from the subject
				if (!ActualFoo::GetFrom(subject,pFoo,ppException))
				{
					// exception is already filled in by GetFrom
					return false;
				}
				
				// instantiate a LS2CodeBoxValue_String, which stores a string value, into the output. use the value of pFoo->m_SomeValue!
				g_pLS2Environment->NewString(pFoo->m_SomeValue.c_str(),(LavishScript2::LS2CodeBoxValue_String **)ppOutput);
				return true;
			}
			static bool __stdcall SetSomeValue(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException)
			{
				LavishScript2::LS2SmartRef<ActualFoo> pFoo;
				// retrieve an ActualFoo from the subject
				if (!ActualFoo::GetFrom(subject,pFoo,ppException))
				{
					// exception is already filled in by GetFrom
					return false;
				}

				LavishScript2::LS2SmartRef<LavishScript2::ILS2String> pString;
				// whatever the input is, grab the string value
				if (!input.GetString(pString,ppException))
				{
					// exception is already filled in by GetString
					return false;
				}

				// replace the previous value of pFoo->m_SomeValue!
				pFoo->m_SomeValue.Assign(pString->c_str());
				return true;
			}


			// An example of a Non-Static Method implementation
			// in this example, a Reference object is passed in, and we set what it references to a new Binary object
			// * this is in contrast to passing in a Binary object and then modifying the object; a Reference can contain any type of value, while a Binary object
			// * can only contain an LS2Buffer
			static bool __stdcall Serialize(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::ILS2Array *pInputs, class LavishScript2::LS2Exception **ppException)
			{
				LavishScript2::LS2SmartRef<ActualFoo> pFoo;
				// retrieve an ActualFoo from the subject
				if (!ActualFoo::GetFrom(subject,pFoo,ppException))
				{
					// exception is already filled in by GetFrom
					return false;
				}

				// ensure that we have exactly one Input, or give an exception and return false
				LS2_REQUIRE_INPUTS(==1);

				// initialize a new variable "outRef", to access input 0 as a Reference value
				LS2_INPUT_AS_REFERENCE(0,outRef);


				// we're going to set up this value to put in the Reference object that was passed in
				LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue_Binary> pBinaryValue;

				// generate a LS2Buffer, which goes in a LS2CodeBoxValue_Binary.
				// in this example we'll serialize the m_SomeValue string, INCLUDING the null-terminator
				LavishScript2::LS2AllocatedBuffer *pBuffer = new LavishScript2::LS2AllocatedBuffer(pFoo->m_SomeValue.GetLength()+1);
				memcpy(pBuffer->m_Buffer,pFoo->m_SomeValue.c_str(),pFoo->m_SomeValue.GetLength()+1);

				// constructs the LS2CodeBoxValue_Binary
				g_pLS2Environment->NewBinary(*pBuffer,pBinaryValue);
				
				// now we don't need this reference anymore, we will refer instead to pBinaryValue
				pBuffer->Delete();
				pBuffer=0;

				// update the Reference object
				outRef->SetReference(pBinaryValue);
				return true;
			}

			// An example implementation of a static "New" method, which constructs a new instance of ActualFoo
			// Note: this is unrelated to the NEWOBJECT instruction, which uses the allocation functionality of ILS2CodeBoxType
			static bool __stdcall New(LavishScript2::ILS2Array *pInputs, class LavishScript2::LS2Exception **ppException)
			{
				// ensure that we have at least one Input, or give an exception and return false
				LS2_REQUIRE_INPUTS(>=1);

				// view input 0 as a reference value
				LS2_INPUT_AS_REFERENCE(0,outRef);
				// slightly longer form
				//LS2_INPUT_AS(0,LavishScript2::VT_Reference,class LavishScript2::LS2CodeBoxValue_Reference,outRef);

				// very much longer form
				/*				
				LS2SmartRef<LavishScript2::LS2CodeBoxValue> refoutRef;
				if (!pInputs->GetAt(0,refoutRef))	
				{
					*ppException = new LavishScript2::LS2OutOfRangeException("No parameter 0"); 
					return false;
				}
				if (!LS2_VALUE_IS(refoutRef,LavishScript2::VT_Reference)) 
				{	
					*ppException = new LavishScript2::LS2InvalidParameterException("Expected VT_Reference as parameter 0");	
					return false;	
				}
				LavishScript2::LS2CodeBoxValue_Reference *outRef = (LavishScript2::LS2CodeBoxValue_Reference *)refoutRef.operator LavishScript2::LS2CodeBoxValue *();
				*/


				// variable for the new object
				ActualFoo *pActualFoo = 0;

				if (pInputs->GetSize()>=2)
				{
					// optional parameter given!  convert it to string or return false & give exception
					LS2_INPUT_TO_STRING(1,initialValue);

					pActualFoo = new ActualFoo(initialValue->c_str());
					printf("Foo.Foo::New(\"%ls\")\n",initialValue->c_str());
				}
				else
				{
					// no optional parameter given
					pActualFoo = new ActualFoo();
					printf("Foo::New()\n");
				}
								
				// we created an ActualFoo, now set up the Value object and set the reference.
				// we pass false to SetReference because it would otherwise increment the reference count of the new LS2CodeBoxValue_ActualFoo, which we want to remain 1.
				outRef->SetReference(new LS2CodeBoxValue_ActualFoo(*ActualFoo::s_pFooType,pActualFoo),false);
				
				// creating the LS2CodeBoxValue_ActualFoo incremented the reference count of pActualFoo to 2 (it holds a ref). drop it back to 1 because we do not hold this reference.
				pActualFoo->Delete();
				pActualFoo=0;// to demonstrate that we do not hold this reference

				return true;
			}

			// An example implementation of ToString
			//     input[0] is a Reference.
			//          also known as the return value.
			static bool __stdcall ToString(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::ILS2Array *pInputs, class LavishScript2::LS2Exception **ppException)
			{
				LavishScript2::LS2SmartRef<ActualFoo> pFoo;
				// retrieve an ActualFoo from the subject
				if (!ActualFoo::GetFrom(subject,pFoo,ppException))
				{
					// exception is already filled in by GetFrom
					return false;
				}

				// ensure that we have exactly one Input, or give an exception and return false
				LS2_REQUIRE_INPUTS(==1);

				// view the input as a Reference value, or give an exception and return false if this is not the right view
				LS2_INPUT_AS_REFERENCE(0,pReturnValue);

				// replace any existing value of the string, with our own string value
				const wchar_t *new_value = pFoo->m_SomeValue.c_str();

				// set up the new object
				LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue_String> pString;
				g_pLS2Environment->NewString(new_value,pString);

				// and set the reference
				pReturnValue->SetReference(pString,true);
				return true;
			}
		};
		
	};
};


	void RegisterFooType()
	{
		LavishScript2::LS2SmartRef<LavishScript2::ILS2CodeBoxType> pType;

		// Register the type as Foo.Foo, implying the namespace "Foo" and type name "Foo"
		if (!g_pLS2Environment->RegisterType(L"Foo.Foo",0,pType))
		{
			// this should probably never happen
			return;
		}

		// set up the static reference to our type
		ActualFoo::s_pFooType = pType;
		pType->AddRef();


		// Register the ActualFoo Allocator for the NEWOBJECT instruction (if you do not want NEWOBJECT support, do not register an allocator)
		{
			LavishScript2::LS2SmartRef<LavishScript2::ILS2CodeBoxValueAllocator> pAllocator;
			pAllocator.Set(*new ActualFoo::Allocator(),false); // the new object starts with ref count=1, passing false keeps it at ref count=1
			if (!pType->RegisterAllocator(pAllocator)) // ref count will be 2 if the registration is successful, otherwise 1
			{
				// this should probably never happen unless pAllocator was 0. (not in this example code)
				// if an allocator was previously registered, it is now replaced.
			}			
			// pAllocator ref count will be either 1 (from being registered) or 0 (if registration failed) from exiting scope.
		}


		// register the static Foo::New() method, which constructs a Foo instance. this is unrelated to the NEWOBJECT instruction.
		//    input[0] should be a Reference object to store the new object
		//        (what you would consider the return value, in this case an arbitrary Reference we will set to a Foo)
		//    input[1] is an optional string parameter. or rather, an object that a string value will be grabbed from; it could be an integer, etc.
		//        (what you would consider the first parameter to a C++ constructor. because it will be exactly that.)
		//pType->RegisterStaticMethod(L"New",&LavishScript2::Types::Foo::New);
		REGISTER_STATIC_METHOD(pType,L"New{}",&LavishScript2::Types::Foo::New,INPUTDECLS(RETURNDECL(L"Foo.Foo")));

		REGISTER_STATIC_METHOD(pType,L"New{string}",&LavishScript2::Types::Foo::New,INPUTDECLS(RETURNDECL(L"Foo.Foo"),INPUTDECL(L"fooValue",L"System.String",L"System.String fooValue")));

		// register the static Foo::Bar() method, which is technically the same as System.Console.Print()
		//     accepts any number of inputs, and prints each one's string value.
		REGISTER_STATIC_METHOD(pType,L"Bar{params string[]}",&LavishScript2::Types::Foo::Bar,INPUTDECLS(INPUTDECL(L"args",L"System.Array",L"params string[] args")));
		

		// register the Foo.Serialize() method
		REGISTER_METHOD(pType,L"Serialize{}",&LavishScript2::Types::Foo::Serialize,INPUTDECLS(RETURNDECL(L"LavishScript2.Binary")));

		// register the Foo.ToString() method (this is the standardized form of ToString.)
		//REGISTER_METHOD(pType,L"ToString{}",&LavishScript2::Types::Foo::ToString,INPUTDECLS(RETURNDECL(L"System.String")));
		REGISTER_METHOD_TOSTRING(pType,&LavishScript2::Types::Foo::ToString);

		// register the Foo.SomeValue property
		pType->RegisterProperty(L"SomeValue",L"System.String",0,&LavishScript2::Types::Foo::GetSomeValue,&LavishScript2::Types::Foo::SetSomeValue);

	}

	void UnregisterFooType()
	{
		// free our reference to it
		ActualFoo::s_pFooType->Delete();
		ActualFoo::s_pFooType=0;

		// unregister the type. it's probably wiser not to unregister the type, but to instead unregister any methods and properties WE added.
		LavishScript2::LS2SmartRef<LavishScript2::LS2Exception> pException;
		g_pLS2Environment->UnregisterType(L"Foo",pException);
	}


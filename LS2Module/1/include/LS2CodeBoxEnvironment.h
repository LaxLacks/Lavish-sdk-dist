#pragma once
#include "LS2CodeBoxValue.h"
#include "LS2String.h"
#include <Windows.h>

namespace LavishScript2
{
	class ILS2CodeBoxType;
	class ILS2Array;
	class ILS2Table;
	class LS2CodeBoxValue;

	// LS2SmartRef is a smart pointer with LS2 reference counting, calling AddRef() and Delete() to inc/dec
	template<class T>
	class LS2SmartRef
	{
	public:
		// pass false as bAddRef if this should not call AddRef (because sometimes you would then need to explicitly Delete on this object, and this should be more optimal)
		__inline LS2SmartRef(T &Object, bool bAddRef=true)
		{
			pObject=&Object;
			if (bAddRef)
				Object.AddRef();
		}
		// pass false as bAddRef if this should not call AddRef (because sometimes you would then need to explicitly Delete on this object, and this should be more optimal)
		__inline LS2SmartRef(T *Object, bool bAddRef=true)
		{
			pObject=Object;
			if (bAddRef)
				Object->AddRef();
		}
		__inline LS2SmartRef()
		{
			pObject=0;
		}

		__inline ~LS2SmartRef()
		{
			Clear();
		}

		__inline bool operator !()
		{
			return !pObject;
		}

		__inline T* operator->()
		{
			return (T*)pObject;
		}

		// explicitly call this operator to use the direct pointer to pObject
		__inline operator T*()
		{
			return (T*)pObject;
		}

		__inline operator T&()
		{
			return (T&)*pObject;
		}

		// reference safety is built into this, it's heavily used. __inline should hopefully optimize out known instances where Clear need not be called.
		__inline operator T**()
		{
			Clear();
			return (T**)&pObject;
		}

		// assigns a new object to the smart pointer. pass false as bAddRef if this should not call AddRef (because sometimes you would then need to explicitly Delete on this object, and this should be more optimal)
		__inline void Set(T &Object, bool bAddRef=true)
		{
			if (bAddRef)
				Object.AddRef();
			if (pObject)
				pObject->Delete();
			pObject=&Object;
		}

		// clears the object in the smart pointer.
		__inline void Clear()
		{
			if (pObject)
			{
				pObject->Delete();
				pObject=0;
			}
		}

	protected:
		T *pObject;
	};

	// a "standard" typeless enumeration callback
	typedef bool (__stdcall *fEnumCallback)(void *object, void *pass_thru);
	typedef bool (__stdcall *fKeyedEnumCallback)(const wchar_t *key,void *object, void *pass_thru);

	// enumerating an array or table will use these callbacks
	typedef bool(__stdcall *fArrayEnum)(size_t nValue, class LavishScript2::LS2CodeBoxValue &value, void *pass_thru);
	typedef bool (__stdcall *fTableEnum)(const wchar_t *key, class LavishScript2::LS2CodeBoxValue &value, void *pass_thru);


	// the base environment, consumed internally by the VM
	class ILS2CodeBoxEnvironment
	{
	public:
		virtual bool BootStrapLS2IL(const void *ls2il_code, size_t ls2il_buflen_in_bytes, LavishScript2::LS2Exception **ppException) = 0;

		// types!

		// registers a new type with a given name. include the "namespace" in the type name. if no parent, pass 0 for pParent. ppType will be the new type. use a smart ref or call Delete() when you're done with it.
		virtual bool RegisterType(const wchar_t *name, LavishScript2::ILS2CodeBoxType *pParent, LavishScript2::ILS2CodeBoxType **ppType)=0;

		// resolves a type by a given name. ppType will be the new type. use a smart ref or call Delete() when you're done with it.
		virtual bool ResolveType(const wchar_t *name, LavishScript2::ILS2CodeBoxType **ppType)=0;
		
		// returns the number of times enum_func was called with an ILS2CodeBoxType
		virtual size_t EnumTypes(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumTypes(LavishScript2::ILS2Table &populate_table)=0;

		// unregistering a type itself is debatable.
		virtual bool UnregisterType(const wchar_t *name, LavishScript2::LS2Exception **ppException)=0;


		// Constructors for built-in object types.. 
		virtual void NewString8(unsigned int code_page, const char *text, LavishScript2::ILS2String8 **ppString)=0;
		virtual void NewString8(unsigned int code_page, const char *text, size_t text_partial_length, LavishScript2::ILS2String8 **ppString)=0;
		virtual bool NewString8(unsigned int code_page, const wchar_t *text, LavishScript2::ILS2String8 **ppString, LavishScript2::LS2Exception **ppException)=0;
		virtual bool NewString8(unsigned int code_page, const wchar_t *text, size_t text_partial_length, LavishScript2::ILS2String8 **ppString, LavishScript2::LS2Exception **ppException)=0;


		virtual bool NewString(unsigned int code_page, const char *text, LavishScript2::ILS2String **ppString, LavishScript2::LS2Exception **ppException)=0;
		virtual bool NewString(unsigned int code_page, const char *text, size_t text_partial_length, LavishScript2::ILS2String **ppString, LavishScript2::LS2Exception **ppException)=0;

		virtual bool NewString(unsigned int code_page, const char *text, LavishScript2::LS2CodeBoxValue_String **ppString, LavishScript2::LS2Exception **ppException)=0;
		virtual bool NewString(unsigned int code_page, const char *text, size_t text_partial_length, LavishScript2::LS2CodeBoxValue_String **ppString, LavishScript2::LS2Exception **ppException)=0;
		virtual void NewString(const wchar_t *text, LavishScript2::LS2CodeBoxValue_String **ppString)=0;
		virtual void NewString(const wchar_t *text, size_t text_partial_length, LavishScript2::LS2CodeBoxValue_String **ppString)=0;

		virtual void NewString(LavishScript2::ILS2String &in_string, LavishScript2::LS2CodeBoxValue_String **ppString)=0;

		virtual void NewBoolean(bool value, LavishScript2::LS2CodeBoxValue_Boolean **ppBoolean)=0;
		virtual void NewChar(wchar_t value, LavishScript2::LS2CodeBoxValue_Char **ppChar)=0;
		virtual void NewEnum(__int64 value, LavishScript2::ILS2CodeBoxType &in_enum_type, LavishScript2::LS2CodeBoxValue_Enum **ppEnum)=0;
		virtual void NewInteger(__int64 value, LavishScript2::eLS2CodeBoxValueType value_type, LavishScript2::LS2CodeBoxValue **ppInteger)=0;
		virtual void NewDecimal(double value, LavishScript2::eLS2CodeBoxValueType value_type, LavishScript2::LS2CodeBoxValue **ppDecimal)=0;

		virtual void NewIntPtr(intptr_t value, LavishScript2::LS2CodeBoxValue_IntPtr **ppIntPtr) = 0;

		virtual void NewNull(LavishScript2::LS2CodeBoxValue_Null **ppNull)=0;
		virtual void NewBinary(LavishScript2::LS2Buffer &in_buffer, LavishScript2::LS2CodeBoxValue_Binary **ppBinary)=0;
		virtual void NewReference(LavishScript2::LS2CodeBoxValue &value, LavishScript2::LS2CodeBoxValue_Reference **ppReference)=0;
		
		virtual void NewArray(LavishScript2::ILS2Array **ppArray)=0;
		virtual void NewTable(LavishScript2::ILS2Table **ppTable)=0;

		virtual void NewArray(LavishScript2::ILS2Array &in_array, LavishScript2::LS2CodeBoxValue_Array **ppArray)=0;
		virtual void NewTable(LavishScript2::ILS2Table &in_Table, LavishScript2::LS2CodeBoxValue_Table **ppTable)=0;

		virtual void NewType(LavishScript2::ILS2CodeBoxType &in_type, LavishScript2::LS2CodeBoxValue_Type **ppType)=0;
		virtual void NewMethod(LavishScript2::ILS2CodeBoxMethod &in_method, LavishScript2::LS2CodeBoxValue_Method **ppMethod)=0;
		virtual void NewStaticMethod(LavishScript2::ILS2CodeBoxStaticMethod &in_method, LavishScript2::LS2CodeBoxValue_StaticMethod **ppStaticMethod)=0;
		virtual void NewProperty(LavishScript2::ILS2CodeBoxProperty &in_property, LavishScript2::LS2CodeBoxValue_Property **ppProperty)=0;
		virtual void NewStaticProperty(LavishScript2::ILS2CodeBoxStaticProperty &in_property, LavishScript2::LS2CodeBoxValue_StaticProperty **ppStaticProperty)=0;

		virtual void NewDelegate(LavishScript2::ILS2CodeBoxType &in_delegateType, LavishScript2::ILS2CodeBoxStaticMethod &in_method, LavishScript2::LS2CodeBoxValue_Delegate **ppDelegate) = 0;
		virtual void NewDelegate(LavishScript2::ILS2CodeBoxType &in_delegateType, LavishScript2::LS2CodeBoxValue &in_subject, LavishScript2::ILS2CodeBoxMethod &in_method, LavishScript2::LS2CodeBoxValue_Delegate **ppDelegate) = 0;
		virtual void NewDelegate(LavishScript2::ILS2CodeBoxDelegate &in_delegate, LavishScript2::LS2CodeBoxValue_Delegate **ppDelegate) = 0;

		virtual void NewLS2ManagedObject(LavishScript2::ILS2CodeBoxManagedObject &in_object, LavishScript2::LS2CodeBoxValue_LS2ManagedObject **ppLS2ManagedObject)=0;

		

		static ILS2CodeBoxEnvironment *s_pInstance;
	};

	class ILS2KeyValuePair
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual bool GetKey(LavishScript2::LS2CodeBoxValue **ppValue)=0;
		virtual bool GetValue(LavishScript2::LS2CodeBoxValue **ppValue)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2KeyValuePair*> LS2CodeBoxValue_KeyValuePair;

	class LS2KeyValuePair : public LavishScript2::ILS2KeyValuePair
	{
	public:
		LS2KeyValuePair(LavishScript2::LS2CodeBoxValue *pKey, LavishScript2::LS2CodeBoxValue *pValue)
		{
			m_RefCount = 1;
			m_pKey = pKey;
			if (pKey)
				pKey->AddRef();

			m_pValue = pValue;
			if (pValue)
				pValue->AddRef();
		}
		~LS2KeyValuePair()
		{
			if (m_pKey)
			{
				m_pKey->Delete();
				m_pKey=0;
			}
			if (m_pValue)
			{
				m_pValue->Delete();
				m_pValue=0;
			}
		}

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

		int m_RefCount;
		LavishScript2::LS2CodeBoxValue *m_pKey;
		LavishScript2::LS2CodeBoxValue *m_pValue;

		virtual bool GetKey(LavishScript2::LS2CodeBoxValue **ppValue)
		{
			if (!m_pKey)
				return false;

			m_pKey->AddRef();
			*ppValue = m_pKey;
			return true;
		}

		virtual bool GetValue(LavishScript2::LS2CodeBoxValue **ppValue)
		{
			if (!m_pValue)
				return false;

			m_pValue->AddRef();
			*ppValue = m_pValue;
			return true;
		}
	};

	class ILS2Enumerator
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual void Dispose()=0;
		virtual bool GetCurrent(LavishScript2::LS2CodeBoxValue **ppValue,LavishScript2::LS2Exception **ppException)=0;
        virtual bool MoveNext(LavishScript2::LS2Exception **ppException__null_if_end_of_enumerable)=0;// may return false and provide no exception under normal conditions
        virtual bool Reset(LavishScript2::LS2Exception **ppException)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2Enumerator*> LS2CodeBoxValue_Enumerator;
#define NATIVE_EXCEPTIONS
#ifdef NATIVE_EXCEPTIONS
	class LS2Exception
	{
	public:
		LS2Exception(ILS2CodeBoxManagedObject &exception_object);
		LS2Exception(const wchar_t *exception_type);
//		LS2Exception(const wchar_t *exception_type, const wchar_t *message);
		LS2Exception(const wchar_t *exception_type, const wchar_t *message_format, ...);
		virtual ~LS2Exception();

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

		virtual bool Tag(const wchar_t *string);
		virtual bool Tag(LavishScript2::ILS2String &in_string);
		virtual bool GetTagArray(LavishScript2::ILS2Array **ppArray);

		virtual bool GetMsg(ILS2String **ppString);
		virtual bool SetMsg(const wchar_t *message);

		virtual bool GetType(LavishScript2::ILS2CodeBoxEnvironment &environment, LavishScript2::ILS2CodeBoxType **ppType);
		virtual bool GetManagedObject(ILS2CodeBoxManagedObject **ppManagedObject, LavishScript2::LS2Exception **ppException);
		ILS2CodeBoxManagedObject *m_pManagedObject;
		ILS2String *m_pExceptionType;
	};

	// Generally means an error in the bytecode construction, but it's probably currently overused...
	class LS2ByteCodeException : public LS2Exception
	{
	public:
		LS2ByteCodeException(const wchar_t *txt) : LS2Exception(L"LavishScript2.ByteCodeException",txt)
		{
			OutputDebugStringW(txt);
		}
	};

	class LS2NoFunctionException : public LS2Exception
	{
	public:
		LS2NoFunctionException(const wchar_t *msg) : LS2Exception(L"LavishScript2.NoFunctionException",L"No function: %ls",msg) { }
	};

	class LS2NoTypeException : public LS2Exception
	{
	public:
		LS2NoTypeException(const wchar_t *msg) : LS2Exception(L"LavishScript2.NoTypeException", L"No Type: %ls", msg) { }
	};

	class LS2NotFoundException : public LS2Exception
	{
	public:
		LS2NotFoundException(const wchar_t *msg) : LS2Exception(L"LavishScript2.NotFoundException",L"Not found: %ls",msg) { }
	};
	class LS2IllegalOperandException : public LS2Exception
	{
	public:
		LS2IllegalOperandException(const wchar_t *msg) : LS2Exception(L"LavishScript2.IllegalOperandException", L"Illegal operand for instruction: %ls", msg) { }
	};

	// 
	class LS2DivideByZeroException : public LS2Exception
	{
	public:
		LS2DivideByZeroException(const wchar_t *msg) : LS2Exception(L"System.DivideByZeroException",L"Divide by zero: %ls",msg) { }
	};

	class LS2InvalidParameterException : public LS2Exception
	{
	public:
		LS2InvalidParameterException(const wchar_t *msg) : LS2Exception(L"LavishScript2.InvalidParameterException",L"Invalid parameter: %ls",msg) { }
	};

	class LS2NotStoppedException : public LS2Exception
	{
	public:
		LS2NotStoppedException(const wchar_t *msg) : LS2Exception(L"LavishScript2.NotStoppedException", L"Not stopped: %ls", msg) { }
	};

	class LS2NoEnvironmentException : public LS2Exception
	{
	public:
		LS2NoEnvironmentException(const wchar_t *msg) : LS2Exception(L"LavishScript2.NoEnvironmentException", L"No environment: %ls", msg) { }
	};

	class LS2AlreadyExistsException : public LS2Exception
	{
	public:
		LS2AlreadyExistsException(const wchar_t *msg) : LS2Exception(L"LavishScript2.AlreadyExistsException", L"Already exists: %ls", msg) { }
	};

	class LS2NotImplementedException : public LS2Exception
	{
	public:
		LS2NotImplementedException(const wchar_t *msg) : LS2Exception(L"System.NotImplementedException", L"Not implemented: %ls", msg) { }
	};

	class LS2IllegalSubjectException : public LS2Exception
	{
	public:
		LS2IllegalSubjectException(const wchar_t *msg) : LS2Exception(L"LavishScript2.IllegalSubjectException", L"Illegal 'this' object: %ls", msg) { }
	};

	class LS2OutOfRangeException : public LS2Exception
	{
	public:
		LS2OutOfRangeException(const wchar_t *msg) : LS2Exception(L"System.ArgumentOutOfRangeException",L"Argument Out of range: %ls",msg) { }
	};

	class LS2NullReferenceException : public LS2Exception
	{
	public:
		LS2NullReferenceException(const wchar_t *msg) : LS2Exception(L"System.NullReferenceException", L"Null reference: %ls", msg) { }
	};

	class LS2ObjectDisposedException : public LS2Exception
	{
	public:
		LS2ObjectDisposedException(const wchar_t *msg) : LS2Exception(L"System.ObjectDisposedException", L"Object disposed: %ls", msg) { }
	};

	//System.InvalidOperationException
	class LS2InvalidOperationException : public LS2Exception
	{
	public:
		LS2InvalidOperationException(const wchar_t *msg) : LS2Exception(L"LavishScript2.InvalidOperationException", L"Invalid operation: %ls", msg) { }
	};


	class LS2Win32SystemErrorException : public LS2Exception
	{
	public:
		LS2Win32SystemErrorException(unsigned int errorCode, const wchar_t *text) : LS2Exception(L"LavishScript2.Win32SystemErrorException")
		{
			m_ErrorCode = errorCode;

			LS2SmartRef<ILS2String> pString;
			LS2String::Format(pString, L"Win32 System Error %u: %ls", errorCode, text);
			SetMsg(pString->c_str());
		}

		unsigned int m_ErrorCode;
	};
	class LS2ThreadStopException : public LS2Exception
	{
	public:
		LS2ThreadStopException(const wchar_t *msg) : LS2Exception(L"LavishScript2.ThreadStopException", L"Thread Stop", msg) { }
	};
#else
	// all LS2 exceptions will be dervied from LS2Exception. imagine that.
	class LS2Exception
	{
	public:
		LS2Exception()
		{
			m_RefCount=1;
			m_pTagArray=0;
			m_pMetaTable=0;
		}
		virtual ~LS2Exception();

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
		
		ILS2Array *m_pTagArray;
		ILS2Table *m_pMetaTable;
		virtual bool Tag(const wchar_t *string);
		virtual bool Tag(LavishScript2::ILS2String &in_string);
		virtual bool GetTagArray(LavishScript2::ILS2Array **ppArray);

		virtual bool GetString(LavishScript2::ILS2String **ppString)=0;

		// determines the LavishScript 2.0 type of the exception
		virtual bool GetType(LavishScript2::ILS2CodeBoxEnvironment &environment, LavishScript2::ILS2CodeBoxType **ppType)
		{
			return environment.ResolveType(L"System.Exception",ppType);
		}

		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable);
	};

	// Simple base class for a string-based exception
	class LS2StringException : public LS2Exception
	{
	public:
		LS2StringException()
		{
		}

		LS2StringException(const wchar_t *txt):m_Text(txt)
		{
		
		}
		LS2StringException(const wchar_t *identifier, const wchar_t *optional_txt):m_Text(identifier)
		{
			
			if (optional_txt && optional_txt[0])
			{
				m_Text.Append(L": ");
				m_Text.Append(optional_txt);
			}
		}

		LS2String m_Text;

		// duplicating preserves the original value of the string.
		virtual bool GetString(LavishScript2::ILS2String **ppString)
		{
			*ppString = new LS2String(m_Text.c_str());
			return true;
/*
			m_Text.AddRef();
			*ppString = &m_Text;
			return true;
*/
		}
	};

	class LS2Win32SystemErrorException : public LS2StringException
	{
	public:
		LS2Win32SystemErrorException(unsigned int errorCode, const wchar_t *text)
		{
			m_ErrorCode = errorCode;
			wchar_t temp[32];
			_snwprintf_s(temp,32,L"%u",errorCode);
			m_Text.Assign(L"Win32 System Error ");
			m_Text.Append(temp);
			m_Text.Append(L": ");
			m_Text.Append(text);
		}

		unsigned int m_ErrorCode;
	};

	class LS2ThreadStopException : public LS2StringException
	{
	public:
		LS2ThreadStopException(const wchar_t *msg) : LS2StringException(L"Thread Stop",msg) { }
	};

	// Simple base class for an exception with an arbitrary value. probably incredibly useful for scripts..
	class LS2ValueException : public LS2Exception
	{
	public:
		LS2ValueException(LS2CodeBoxValue &value) : m_Value(value)
		{
			value.AddRef();
		}
		virtual ~LS2ValueException()
		{
			m_Value.Delete();
		}

		LS2CodeBoxValue &m_Value;

		virtual bool GetString(LavishScript2::ILS2String **ppString)
		{
			LavishScript2::LS2Exception *pException = 0;
			if (!m_Value.GetString(ppString,&pException))
			{
				pException->Delete();
				return false;
			}
			return true;
		}

		// determines the LavishScript 2.0 type of the exception
		virtual bool GetType(LavishScript2::ILS2CodeBoxEnvironment &environment, LavishScript2::ILS2CodeBoxType **ppType)
		{
			return environment.ResolveType(L"Exception",ppType);
		}

		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)
		{
			return m_Value.GetMetaTable(ppTable);
		}
	};

	// Generally means an error in the bytecode construction, but it's probably currently overused...
	class LS2ByteCodeException : public LS2StringException
	{
	public:
		LS2ByteCodeException(const wchar_t *txt) : LS2StringException(txt)
		{
		}
	};

	// 
	class LS2DivideByZeroException : public LS2StringException
	{
	public:
		LS2DivideByZeroException(const wchar_t *msg) : LS2StringException(L"Divide by zero",msg) { }
	};

	class LS2IllegalOperandException : public LS2StringException
	{
	public:
		LS2IllegalOperandException(const wchar_t *msg) : LS2StringException(L"Illegal operand for instruction",msg) 
		{ 
		
		}
	};

	class LS2InvalidParameterException : public LS2StringException
	{
	public:
		LS2InvalidParameterException(const wchar_t *msg) : LS2StringException(L"Invalid parameter",msg) { }
	};

	class LS2NotFoundException : public LS2StringException
	{
	public:
		LS2NotFoundException(const wchar_t *msg) : LS2StringException(L"Not found",msg) { }
	};

	class LS2NotStoppedException : public LS2StringException
	{
	public:
		LS2NotStoppedException(const wchar_t *msg): LS2StringException(L"Not stopped",msg) { }
	};

	class LS2NoEnvironmentException : public LS2StringException
	{
	public:
		LS2NoEnvironmentException(const wchar_t *msg) : LS2StringException(L"No environment",msg) { }
	};

	class LS2NoFunctionException : public LS2StringException
	{
	public:
		LS2NoFunctionException(const wchar_t *msg) : LS2StringException(L"No function",msg) { }
	};

	class LS2AlreadyExistsException : public LS2StringException
	{
	public:
		LS2AlreadyExistsException(const wchar_t *msg) : LS2StringException(L"Already exists",msg) { }
	};

	class LS2NotImplementedException : public LS2StringException
	{
	public:
		LS2NotImplementedException(const wchar_t *msg) : LS2StringException(L"Not implemented",msg) { }
	};

	class LS2IllegalSubjectException : public LS2StringException
	{
	public:
		LS2IllegalSubjectException(const wchar_t *msg) : LS2StringException(L"Illegal 'this' object",msg) { }
	};

	class LS2OutOfRangeException : public LS2StringException
	{
	public:
		LS2OutOfRangeException(const wchar_t *msg) : LS2StringException(L"Out of range",msg) { }
	};

	class LS2NullReferenceException : public LS2StringException
	{
	public:
		LS2NullReferenceException(const wchar_t *msg) : LS2StringException(L"Null reference",msg) { }
	};

	class LS2ObjectDisposedException : public LS2StringException
	{
	public:
		LS2ObjectDisposedException(const wchar_t *msg) : LS2StringException(L"Object disposed",msg) { }
	};

	//System.InvalidOperationException
	class LS2InvalidOperationException : public LS2StringException
	{
	public:
		LS2InvalidOperationException(const wchar_t *msg) : LS2StringException(L"Invalid operation",msg) { }
	};
#endif

	// arbtirary reference counted memory region
	class LS2Buffer
	{
	public:
		LS2Buffer(void *buf, size_t buflen);
		virtual ~LS2Buffer();

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

		void *m_Buffer;
		size_t m_BufferLength;
	};

	// like LS2Buffer except self-allocated and freed
	class LS2AllocatedBuffer : public LS2Buffer
	{
	public:
		LS2AllocatedBuffer(size_t size);
		virtual ~LS2AllocatedBuffer();
	};

	// interface used by arrays. 0-based.
	class ILS2Array
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		// get the capacity of the array (e.g. List.Capacity)
		virtual size_t GetCapacity() = 0;
		// get the number of values in the array (e.g. List.Count)
		virtual size_t GetCount() = 0;

		// resize the array, releasing objects as needed at the end to shrink, or adding Nulls at the end to expand
		virtual bool Resize(size_t newSize, LavishScript2::ILS2CodeBoxType *pType_optional, LavishScript2::LS2Exception **ppException) = 0;
		// resize the array "backwards", releasing objects as needed at the front to shrink, or inserting Nulls at the front to expand. "0" is expected to move.
		virtual bool ResizeBackwards(size_t newSize, LavishScript2::ILS2CodeBoxType *pType_optional, LavishScript2::LS2Exception **ppException) = 0;

		// retrieve the value at nValue. expected to only return false if nValue is out of range
		virtual bool GetAt(size_t nValue, LavishScript2::LS2CodeBoxValue **ppValue) = 0;
		// replace the value at nValue. expected to only return false if nValue is out of range
		virtual bool SetAt(size_t nValue, LavishScript2::LS2CodeBoxValue &value) = 0;

		// create a new spot by pushing toward the end, and set it to this value
		virtual bool InsertAt(size_t nValue, LavishScript2::LS2CodeBoxValue &value) = 0;

		virtual bool Append(LavishScript2::LS2CodeBoxValue &value)=0;
		virtual bool Append(LavishScript2::ILS2Array &values)=0;

		// set a range of nLength values, starting at nValue. use the values of _array starting at _index
		virtual bool SetRange(size_t nValue, size_t nLength, LavishScript2::ILS2Array &_array, size_t _index) = 0;

		// enumerate the values starting with nStartAt, by calling back through enum_func. pass_thru is a user-defined value to be pass'd thru to enum_func 
		virtual bool Enumerate(size_t nStartAt, LavishScript2::fArrayEnum enum_func, void *pass_thru) = 0;
		virtual bool GetEnumerator(unsigned int enumerator_flags, LavishScript2::ILS2Enumerator **ppEnumerator, LavishScript2::LS2Exception **ppException)=0;

		virtual bool Equal(LavishScript2::ILS2Array &compare_to)=0;

		// GetAt with simplified value conversion
		template<class T>
		bool GetAt(size_t nValue, enum LavishScript2::eLS2CodeBoxValueType value_type, T **ppValue, LavishScript2::LS2Exception **ppException)
		{
			LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue> pValue;
			if (!GetAt(nValue,pValue))
			{
				wchar_t temp[256];
				_snwprintf_s(temp, 256, L"array value out of range {nValue=%u Count=%u}", (unsigned int)nValue, (unsigned int)GetCount());
				*ppException = new LavishScript2::LS2OutOfRangeException(temp);
				return false;
			}

			if (pValue->m_Type!=value_type)
			{
				wchar_t temp[256];
				_snwprintf_s(temp,256,L"Expected value type %ls but got %ls",LS2CodeBoxValue::GetValueTypeName(value_type),LS2CodeBoxValue::GetValueTypeName(pValue->m_Type));

				if (pValue->m_Type==VT_Null)
					*ppException = new LavishScript2::LS2NullReferenceException(temp);
				else
					*ppException = new LavishScript2::LS2InvalidParameterException(temp);
				return false;
			}

			*ppValue = (T*)pValue.operator LavishScript2::LS2CodeBoxValue *();
			pValue->AddRef();
			return true;
		}
	};

	// interface used by tables. string key.
	class ILS2Table
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		// get the number of values in the table
		virtual size_t GetSize() = 0;

		// retrieve the value at key. expected to only return false if the key does not exist
		virtual bool GetAt(const wchar_t *key, LavishScript2::LS2CodeBoxValue **ppValue)=0;
		// replace the value at key. expected to only return false if the key is invalid
		virtual bool SetAt(const wchar_t *key, LavishScript2::LS2CodeBoxValue &value)=0;

		virtual bool Erase(const wchar_t *key)=0;

		virtual void Clear()=0;

		// enumerate the values, by calling back through enum_func. pass_thru is a user-defined value to be pass'd thru to enum_func 
		virtual bool Enumerate(LavishScript2::fTableEnum enum_func, void *pass_thru)=0;
		virtual bool GetEnumerator(unsigned int enumerator_flags, LavishScript2::ILS2Enumerator **ppEnumerator, LavishScript2::LS2Exception **ppException)=0;
		
		// GetAt with simplified value conversion
		template<class T>
		bool GetAt(const wchar_t *key,enum LavishScript2::eLS2CodeBoxValueType value_type,T **ppValue,LavishScript2::LS2Exception **ppException)
		{
			LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue> pValue;
			if (!GetAt(key,pValue))
			{
				*ppException = new LavishScript2::LS2OutOfRangeException(L"key not found in table");
				return false;
			}

			if (pValue->m_Type!=value_type)
			{
				wchar_t temp[256];
				_snwprintf_s(temp,256,L"Expected value type %ls but got %ls",LS2CodeBoxValue::GetValueTypeName(value_type),LS2CodeBoxValue::GetValueTypeName(pValue->m_Type));

				if (pValue->m_Type==VT_Null)
					*ppException = new LavishScript2::LS2NullReferenceException(temp);
				else
					*ppException = new LavishScript2::LS2InvalidParameterException(temp);
				return false;
			}

			*ppValue = (T*)pValue.operator LavishScript2::LS2CodeBoxValue *();
			pValue->AddRef();
			return true;
		}
	};

	// callbacks for methods, properties and fields
	typedef bool (__stdcall *fMethod)(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException);
	typedef bool (__stdcall *fStaticMethod)(LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException);
	typedef bool (__stdcall *fPropertyGet)(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException);
	typedef bool (__stdcall *fPropertySet)(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException);
	typedef bool (__stdcall *fStaticPropertyGet)(LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException);
	typedef bool (__stdcall *fStaticPropertySet)(LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException);

	// this is an actual bytecode function
	class ILS2CodeBoxFunction
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;

		virtual bool ExecuteAtomic(LavishScript2::LS2CodeBoxValue *pSubject_may_be_0, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
	};

	class ILS2CodeBoxInputDeclaration
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetName(LavishScript2::ILS2String **ppOutString)=0;
		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetDefaultValue(LavishScript2::LS2CodeBoxValue **ppOutput)=0;

		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2CodeBoxInputDeclaration*> LS2CodeBoxValue_InputDeclaration;

	class LS2CodeBoxInputDeclaration : public LavishScript2::ILS2CodeBoxInputDeclaration
	{
	public:
		LS2CodeBoxInputDeclaration(const wchar_t *name, const wchar_t *type);
		LS2CodeBoxInputDeclaration(const wchar_t *name, const wchar_t *type, const wchar_t *cs_decl);
		LS2CodeBoxInputDeclaration(const wchar_t *name, const wchar_t *type, const wchar_t *cs_decl, LavishScript2::LS2CodeBoxValue *pDefaultValue);
		~LS2CodeBoxInputDeclaration();

		virtual int AddRef();
		virtual int Delete();

		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable);
		virtual bool GetName(LavishScript2::ILS2String **ppOutString);
		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException);
		virtual bool GetDefaultValue(LavishScript2::LS2CodeBoxValue **ppOutput);

		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString);

		int m_RefCount;
		std::wstring m_Type;
		std::wstring m_Name;
		std::wstring m_CSharpDeclaration;
		LavishScript2::LS2CodeBoxValue *m_pDefaultValue;
		LavishScript2::ILS2Table *m_pMetaTable;


		struct simple_declaration_node
		{
			const wchar_t *name;
			const wchar_t *type;
			const wchar_t *cs_decl;
		};
		static bool GenerateDeclarationArray(simple_declaration_node *nodes, size_t num_nodes, LavishScript2::ILS2Array **ppArray);
		static bool GenerateDeclarationArray(LavishScript2::ILS2Array &in_array, LavishScript2::ILS2Array **ppArray, LavishScript2::LS2Exception **ppException);
	};

	enum eLS2DelegateType
	{
		DT_Method,
		DT_StaticMethod,
		DT_List,
	};

	class ILS2CodeBoxDelegate
	{
	public:
		virtual int AddRef() = 0;
		virtual int Delete() = 0;

		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType) = 0;
		virtual bool Execute(LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException) = 0;

		virtual bool GetInvocationList(LavishScript2::ILS2Array **ppArray) = 0;

		virtual enum LavishScript2::eLS2DelegateType GetDelegateType() = 0;
		virtual bool Equal(LavishScript2::ILS2CodeBoxDelegate &value) = 0;
		virtual bool IsEmpty() = 0;

		static bool Combine(LavishScript2::ILS2CodeBoxDelegate *pFirst, LavishScript2::ILS2CodeBoxDelegate &last, LavishScript2::ILS2CodeBoxDelegate **ppOutput);
		static bool Remove(LavishScript2::ILS2CodeBoxDelegate &source, LavishScript2::ILS2CodeBoxDelegate &value, LavishScript2::ILS2CodeBoxDelegate **ppOutput);
	};

	// interfaces for methods and properties, which may or may not be implemented by bytecode functions. you may implement method and property interfaces!
	class ILS2CodeBoxMethod
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetInputDeclarations(LavishScript2::ILS2Array **ppArray, LavishScript2::LS2Exception **ppException)=0;

		virtual bool Execute(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;

		// if the method is implemented by a bytecode Function, this returns it. 
		// your implementation will return false and does not need to alter *ppFunction (but would otherwise set it to 0)
		virtual bool GetILS2CodeBoxFunction(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;

	};

	class ILS2CodeBoxStaticMethod
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetInputDeclarations(LavishScript2::ILS2Array **ppArray, LavishScript2::LS2Exception **ppException)=0;

		virtual bool Execute(LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetILS2CodeBoxFunction(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;
	};

	class ILS2CodeBoxField
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString)=0;
		
		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetName(LavishScript2::ILS2String **ppOutName) = 0;

		virtual bool GetInitialValue(LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)=0;
		virtual size_t GetLocalFieldID() = 0;
		virtual size_t GetRuntimeFieldID(LavishScript2::LS2Exception **ppException) = 0;

		virtual bool Get(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Set(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException)=0;
	};

	class ILS2CodeBoxStaticField
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString)=0;

		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetName(LavishScript2::ILS2String **ppOutName)=0; 

		virtual bool Get(LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Set(LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException)=0;
	};

	class ILS2CodeBoxProperty
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString)=0;

		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException)=0;
		virtual bool IsGetSupported()=0;
		virtual bool IsSetSupported()=0;

		virtual bool Get(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetILS2CodeBoxFunction_Get(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;

		virtual bool Set(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetILS2CodeBoxFunction_Set(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;

		// Get with simplified value conversion
		template<class T>
		bool Get(LavishScript2::LS2CodeBoxValue &subject, enum LavishScript2::eLS2CodeBoxValueType value_type, T **ppValue, LavishScript2::LS2Exception **ppException)
		{
			LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue> pValue;
			if (!Get(subject, pValue, ppException))
			{
				return false;
			}

			if (pValue->m_Type != value_type)
			{
				wchar_t temp[256];
				_snwprintf_s(temp, 256, L"Expected value type %ls but got %ls", LS2CodeBoxValue::GetValueTypeName(value_type), LS2CodeBoxValue::GetValueTypeName(pValue->m_Type));

				if (pValue->m_Type == VT_Null)
					*ppException = new LavishScript2::LS2NullReferenceException(temp);
				else
					*ppException = new LavishScript2::LS2InvalidParameterException(temp);
				return false;
			}

			*ppValue = (T*)pValue.operator LavishScript2::LS2CodeBoxValue *();
			pValue->AddRef();
			return true;
		}
	};

	class ILS2CodeBoxStaticProperty
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppOutType)=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
		virtual bool GetCSharpDeclaration(LavishScript2::ILS2String **ppOutString)=0;

		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType, LavishScript2::LS2Exception **ppException)=0;
		virtual bool IsGetSupported()=0;
		virtual bool IsSetSupported()=0;

		virtual bool Get(LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetILS2CodeBoxFunction_Get(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;

		virtual bool Set(LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetILS2CodeBoxFunction_Set(LavishScript2::ILS2CodeBoxFunction **ppFunction)=0;
	};

	

	class ILS2CodeBoxValueAllocator
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;

		virtual bool Allocate(LavishScript2::ILS2CodeBoxMethod *pConstructor_optional, LavishScript2::ILS2Array *pInputs_optional, LavishScript2::LS2CodeBoxValue **ppNewObject, LavishScript2::LS2Exception **ppException)=0;
	};


	class ILS2CodeBoxManagedObject
	{
	public:
		virtual int AddRef() = 0;
		virtual int Delete() = 0;

		virtual bool GetType(LavishScript2::ILS2CodeBoxType **ppOutType) = 0;
		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable) = 0;

		virtual bool SetField(size_t runtime_field_id, LavishScript2::LS2CodeBoxValue &in_value, LavishScript2::LS2Exception **ppException) = 0;
		virtual bool GetField(size_t runtime_field_id, LavishScript2::LS2CodeBoxValue **ppOutValue, LavishScript2::LS2Exception **ppException) = 0;
		virtual bool GetFieldArray(LavishScript2::ILS2Array **ppArray) = 0;

		bool ResolveFieldID(const wchar_t *name, size_t &out_field_id, LavishScript2::LS2Exception **ppException);

		template<class T>
		bool GetProperty(const wchar_t *property_name, enum LavishScript2::eLS2CodeBoxValueType value_type, T **ppValue, LavishScript2::LS2Exception **ppException)
		{
			LavishScript2::LS2SmartRef<LavishScript2::ILS2CodeBoxType> pType;
			if (!GetType(pType))
			{
				// object without type
				return false;
			}

			LavishScript2::LS2SmartRef<LavishScript2::ILS2CodeBoxProperty> pProperty;
			if (!pType->ResolveProperty(property_name, pProperty))
			{
				return false;
			}

			LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue_LS2ManagedObject> pObjectValue;
			LavishScript2::LS2CodeBoxEnvironment::s_pInstance->NewLS2ManagedObject(*this, pObjectValue);
			return pProperty->Get<T>(pObjectValue, VT_String, ppValue, ppException);
		}


		bool SetProperty(const wchar_t *property_name, LavishScript2::LS2CodeBoxValue &value, LavishScript2::LS2Exception **ppException);


		// GetField with simplified value conversion
		template<class T>
		bool GetField(size_t runtime_field_id, enum LavishScript2::eLS2CodeBoxValueType value_type, T **ppValue, LavishScript2::LS2Exception **ppException);

		// GetField with simplified value conversion
		template<class T>
		bool GetField(const wchar_t *field_name, enum LavishScript2::eLS2CodeBoxValueType value_type, T **ppValue, LavishScript2::LS2Exception **ppException);
	};


	// interface for a Type definition
	class ILS2CodeBoxType
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual void GetName(LavishScript2::ILS2String **ppString)=0;

		virtual bool RegisterAllocator(LavishScript2::ILS2CodeBoxValueAllocator *pAllocator)=0;
		virtual bool GetAllocator(LavishScript2::ILS2CodeBoxValueAllocator **ppOutAllocator)=0;
		virtual bool UnregisterAllocator()=0;

		virtual bool RegisterInterface(LavishScript2::ILS2CodeBoxType &iface)=0;
		virtual bool UnregisterInterface(LavishScript2::ILS2CodeBoxType &iface)=0;

		virtual bool AllocateAndConstructObject(LavishScript2::ILS2CodeBoxMethod *pConstructor, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2CodeBoxValue **ppNewObject, LavishScript2::LS2Exception **ppException)=0;

		virtual size_t GetNumFields() = 0;
		virtual bool RegisterField(const wchar_t *name, const wchar_t *type_name, const wchar_t *cs_decl, LavishScript2::LS2CodeBoxValue *pInitialValue, LavishScript2::ILS2CodeBoxField **ppOutField)=0;
		virtual bool RegisterStaticField(const wchar_t *name, const wchar_t *type_name, const wchar_t *cs_decl, LavishScript2::LS2CodeBoxValue *pInitialValue, LavishScript2::ILS2CodeBoxStaticField **ppOutField)=0;
		virtual bool AllocateFields(LavishScript2::ILS2CodeBoxManagedObject &into_object, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetRuntimeFieldID(size_t local_field_id, size_t &runtime_field_id) = 0;

		virtual bool ResolveEnumValue(__int64 value, LavishScript2::ILS2CodeBoxStaticField **ppOutField, LavishScript2::LS2Exception **ppException)=0;
		virtual bool ResolveField(size_t local_field_id, LavishScript2::ILS2CodeBoxField **ppOutField) = 0;
		virtual bool ResolveField(const wchar_t *name, LavishScript2::ILS2CodeBoxField **ppOutField) = 0;
		virtual bool ResolveStaticField(const wchar_t *name, LavishScript2::ILS2CodeBoxStaticField **ppOutField)=0;
		virtual bool ResolveMethod(const wchar_t *name, LavishScript2::ILS2CodeBoxMethod **ppOutMethod)=0;
		virtual bool ResolveStaticMethod(const wchar_t *name, LavishScript2::ILS2CodeBoxStaticMethod **ppOutMethod)=0;
		virtual bool ResolveProperty(const wchar_t *name, LavishScript2::ILS2CodeBoxProperty **ppOutProperty)=0;
		virtual bool ResolveStaticProperty(const wchar_t *name, LavishScript2::ILS2CodeBoxStaticProperty **ppOutProperty)=0;

		virtual bool RegisterMethod(const wchar_t *name, LavishScript2::ILS2CodeBoxMethod *pMethod)=0;
		virtual bool RegisterStaticMethod(const wchar_t *name, LavishScript2::ILS2CodeBoxStaticMethod *pMethod)=0;
		virtual bool RegisterProperty(const wchar_t *name, LavishScript2::ILS2CodeBoxProperty *pProperty)=0;
		virtual bool RegisterStaticProperty(const wchar_t *name, LavishScript2::ILS2CodeBoxStaticProperty *pProperty)=0;

		virtual bool RegisterMethod(const wchar_t *name, LavishScript2::ILS2Array *pInputDeclarations, LavishScript2::fMethod method)=0;
		virtual bool RegisterStaticMethod(const wchar_t *name, LavishScript2::ILS2Array *pInputDeclarations, LavishScript2::fStaticMethod method)=0;
		virtual bool RegisterProperty(const wchar_t *name, const wchar_t *type_name, const wchar_t *cs_decl_or_auto, LavishScript2::fPropertyGet getter, LavishScript2::fPropertySet setter) = 0;
		virtual bool RegisterStaticProperty(const wchar_t *name, const wchar_t *type_name, const wchar_t *cs_decl_or_auto, LavishScript2::fStaticPropertyGet getter, LavishScript2::fStaticPropertySet setter) = 0;

		virtual size_t EnumMethods(LavishScript2::fKeyedEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumStaticMethods(LavishScript2::fKeyedEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumProperties(LavishScript2::fKeyedEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumStaticProperties(LavishScript2::fKeyedEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumFields(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumStaticFields(LavishScript2::fKeyedEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumInterfaces(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;


		virtual size_t EnumMethods(LavishScript2::ILS2Table &populate_table)=0;
		virtual size_t EnumStaticMethods(LavishScript2::ILS2Table &populate_table)=0;
		virtual size_t EnumProperties(LavishScript2::ILS2Table &populate_table)=0;
		virtual size_t EnumStaticProperties(LavishScript2::ILS2Table &populate_table)=0;
		virtual size_t EnumFields(LavishScript2::ILS2Array &populate_array)=0;
		virtual size_t EnumStaticFields(LavishScript2::ILS2Table &populate_table)=0;
		virtual size_t EnumInterfaces(LavishScript2::ILS2Array &populate_array)=0;
		
		virtual bool UnregisterMethod(const wchar_t *name)=0;
		virtual bool UnregisterStaticMethod(const wchar_t *name)=0;
		virtual bool UnregisterProperty(const wchar_t *name)=0;
		virtual bool UnregisterStaticProperty(const wchar_t *name)=0;
		virtual bool UnregisterStaticField(const wchar_t *name)=0;

		virtual bool GetParentType(LavishScript2::ILS2CodeBoxType **ppType)=0;// base type

		virtual bool GetMetaTable(LavishScript2::ILS2Table **ppTable)=0;
//		virtual bool GetStaticFieldTable(LavishScript2::ILS2Table **ppTable)=0;

		virtual bool Is(LavishScript2::ILS2CodeBoxType &compare_type)=0;
		bool Is(const wchar_t *compare_type);
		bool ResolveDefaultConstructor(LavishScript2::ILS2CodeBoxMethod **ppOutMethod);
	};

};

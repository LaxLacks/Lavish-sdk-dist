#pragma once
#include <LS2CodeBoxEnvironment.h>
#include <LS2CodeBoxValue.h>
namespace LavishScript2
{

#define LS2MODULE_API_VERSION	2

	struct LS2BuildInfo
	{													//   40.3.20120827.2
		unsigned char MajorVersion; // major version number  40
		unsigned char MinorVersion; // minor version number  3
		unsigned short ReleaseYear; // YYYY of build		 2012
		unsigned short ReleaseDate;	// MMDD of build         0827
		unsigned char Revision;		// build # on that date  2
	};

		class LS2Event;
		class ILS2StandardEnvironment;
		class ILS2Script;

		// here we have a bunch of macros to simplify handling LavishScript 2.0 objects in your custom Methods and Properties
#define LS2_REQUIRE_NOINPUTS()	\
			if (pInputs && pInputs->GetSize()!=0)\
			{\
				*ppException = new LavishScript2::LS2InvalidParameterException(L"Require NO inputs");\
				return false;\
			}

#define LS2_REQUIRE_INPUTS(_comparison_)	\
			if (!pInputs)	\
			{\
			*ppException = new LavishScript2::LS2InvalidParameterException(L"RequireInputs"##L#_comparison_);	\
				return false;\
			}\
			\
			if (!(pInputs->GetSize() ## _comparison_))\
			{\
			*ppException = new LavishScript2::LS2InvalidParameterException(L"RequireInputs"##L#_comparison_);\
				return false;				\
			}

#define LS2_INPUT_TO_VALUE(_n_,_var_) \
			LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue> _var_;\
			if (!pInputs->GetAt(_n_,_var_))	\
			{\
			*ppException = new LavishScript2::LS2OutOfRangeException(L"No parameter "##L#_n_); \
				return false;\
			}

#define LS2_INPUT_AS(_n_,_valuetype_,_type_,_var_)	\
	LavishScript2::LS2SmartRef<LavishScript2::LS2CodeBoxValue> ref##_var_;\
	if (!pInputs->GetAt(_n_,ref##_var_))	\
			{\
			*ppException = new LavishScript2::LS2OutOfRangeException(L"No parameter "##L#_n_); \
				return false;\
			}\
			if (!LS2_VALUE_IS(ref##_var_,_valuetype_)) \
			{	\
			*ppException = new LavishScript2::LS2InvalidParameterException(L"Expected "##L#_valuetype_##L" as parameter "##L#_n_);	\
				return false;	\
			}\
			_type_ *_var_ = (_type_*)ref##_var_.operator LavishScript2::LS2CodeBoxValue *();

#define LS2_INPUT_TO(_what_,_n_,_type_,_var_)	\
			_type_ _var_;\
			{\
				LavishScript2::LS2Exception *pException = 0;\
				LS2_INPUT_TO_VALUE(_n_,__tempvar);\
				_var_ = (_type_)__tempvar->Get##_what_(&pException);\
				if (pException)\
				{\
					*ppException = pException;\
					return false;\
				}\
			}

#define LS2_INPUT_TO_(_what_,_n_,_type_,_var_)	\
			LavishScript2::LS2SmartRef<_type_> _var_;\
			{\
				LS2_INPUT_TO_VALUE(_n_,__tempvar);\
				if (!__tempvar->Get##_what_(_var_,ppException))\
					return false;\
			}

#define LS2_INPUT_TO_INT(_n_,_type_,_var_)	LS2_INPUT_TO(Int,_n_,_type_,_var_)
#define LS2_INPUT_TO_STRING(_n_,_var_)	LS2_INPUT_TO_(String,_n_,LavishScript2::ILS2String,_var_)


#define LS2_VALUE_IS(_value_,_valuetype_) (_value_->m_Type==_valuetype_)

// PROPINPUT - used for property setter
#define LS2_PROPINPUT_IS(_valuetype_) (input.m_Type==_valuetype_)
#define LS2_PROPINPUT_AS(_valuetype_,_type_,_var_)	\
			if (input.m_Type != _valuetype_) \
			{	\
			*ppException = new LavishScript2::LS2InvalidParameterException(L"Expected "##L#_valuetype_##L" as property value");	\
				return false;	\
			}	\
			_type_ *_var_ = (_type_*)&input;

#define LS2_PROPINPUT_AS_CHAR(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Char,LavishScript2::LS2CodeBoxValue_Char,_var_)
#define LS2_PROPINPUT_AS_STRING(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_String,LavishScript2::LS2CodeBoxValue_String,_var_)
#define LS2_PROPINPUT_AS_BOOLEAN(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Boolean,LavishScript2::LS2CodeBoxValue_Boolean_var_)
#define LS2_PROPINPUT_AS_INT8(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Int8,LavishScript2::LS2CodeBoxValue_Int8_var_)
#define LS2_PROPINPUT_AS_INT16(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Int16,LavishScript2::LS2CodeBoxValue_Int16_var_)
#define LS2_PROPINPUT_AS_INT32(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Int32,LavishScript2::LS2CodeBoxValue_Int32_var_)
#define LS2_PROPINPUT_AS_INT64(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Int64,LavishScript2::LS2CodeBoxValue_Int64_var_)
#define LS2_PROPINPUT_AS_INTPTR(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_IntPtr,LavishScript2::LS2CodeBoxValue_IntPtr_var_)
#define LS2_PROPINPUT_AS_UINT8(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_UInt8,LavishScript2::LS2CodeBoxValue_UInt8_var_)
#define LS2_PROPINPUT_AS_UINT16(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_UInt16,LavishScript2::LS2CodeBoxValue_UInt16_var_)
#define LS2_PROPINPUT_AS_UINT32(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_UInt32,LavishScript2::LS2CodeBoxValue_UInt32_var_)
#define LS2_PROPINPUT_AS_UINT64(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_UInt64,LavishScript2::LS2CodeBoxValue_UInt64_var_)
#define LS2_PROPINPUT_AS_DECIMAL(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Decimal,LavishScript2::LS2CodeBoxValue_Decimal,_var_)
#define LS2_PROPINPUT_AS_DOUBLE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Double,LavishScript2::LS2CodeBoxValue_Double,_var_)
#define LS2_PROPINPUT_AS_SINGLE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Single,LavishScript2::LS2CodeBoxValue_Single,_var_)
#define LS2_PROPINPUT_AS_BINARY(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Binary,LavishScript2::LS2CodeBoxValue_Binary,_var_)
#define LS2_PROPINPUT_AS_ARRAY(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Array,LavishScript2::LS2CodeBoxValue_Array,_var_)
#define LS2_PROPINPUT_AS_TABLE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Table,LavishScript2::LS2CodeBoxValue_Table,_var_)
#define LS2_PROPINPUT_AS_DELEGATE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Delegate,LavishScript2::LS2CodeBoxValue_Delegate,_var_)
#define LS2_PROPINPUT_AS_OBJECT(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Object,LavishScript2::LS2CodeBoxValue_Object,_var_)
#define LS2_PROPINPUT_AS_TYPE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Type,LavishScript2::LS2CodeBoxValue_Type,_var_)
//#define LS2_PROPINPUT_AS_EXCEPTION(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Exception,LavishScript2::LS2CodeBoxValue_Exception,_var_)
#define LS2_PROPINPUT_AS_REFERENCE(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Reference,LavishScript2::LS2CodeBoxValue_Reference,_var_)
#define LS2_PROPINPUT_AS_FUNCTION(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Function,LavishScript2::LS2CodeBoxValue_Function,_var_)
#define LS2_PROPINPUT_AS_METHOD(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Method,LavishScript2::LS2CodeBoxValue_Method,_var_)
#define LS2_PROPINPUT_AS_STATICMETHOD(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_StaticMethod,LavishScript2::LS2CodeBoxValue_StaticMethod,_var_)
#define LS2_PROPINPUT_AS_PROPERTY(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Property,LavishScript2::LS2CodeBoxValue_Property,_var_)
#define LS2_PROPINPUT_AS_STATICPROPERTY(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_StaticProperty,LavishScript2::LS2CodeBoxValue_StaticProperty,_var_)
#define LS2_PROPINPUT_AS_FIELD(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Field,LavishScript2::LS2CodeBoxValue_Field,_var_)
#define LS2_PROPINPUT_AS_STATICFIELD(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_StaticField,LavishScript2::LS2CodeBoxValue_StaticField,_var_)
#define LS2_PROPINPUT_AS_NULL(_var_)	LS2_PROPINPUT_AS(LavishScript2::VT_Null,LavishScript2::LS2CodeBoxValue_Null,_var_)

#define LS2_PROPINPUT_AS_OBJECT_(_objtype_,_type_,_var_)	\
		LS2_PROPINPUT_AS_OBJECT(obj##_var_); \
		if (&obj##_var_->m_Type!=_objtype_) \
		{\
			*ppException = new LavishScript2::LS2IllegalSubjectException(L"Unexpected 'this' object type");\
			return false;\
		}\
		_type_ *_var_ = (_type_*)obj##_var_;


#define LS2_PROPINPUT_TO(_what_,_type_,_var_)	\
			_type_ _var_;\
			{\
				LavishScript2::LS2Exception *pException = 0;\
				_var_ = (_type_)input.Get##_what_(&pException);\
				if (pException)\
				{\
					*ppException = pException;\
					return false;\
				}\
			}
#define LS2_PROPINPUT_TO_(_what_,_type_,_var_)	\
			LavishScript2::LS2SmartRef<_type_> _var_;\
			{\
				if (!input.Get##_what_(_var_,ppException))\
					return false;\
			}

#define LS2_PROPINPUT_TO_INT(_type_,_var_)	LS2_PROPINPUT_TO(Int,_type_,_var_)
#define LS2_PROPINPUT_TO_STRING(_var_)	LS2_PROPINPUT_TO_(String,LavishScript2::ILS2String,_var_)

//
#define LS2_SUBJECT_IS(_valuetype_) (subject.m_Type==_valuetype_)
#define LS2_SUBJECT_AS(_valuetype_,_type_,_var_)	\
			if (subject.m_Type != _valuetype_) \
			{	\
			*ppException = new LavishScript2::LS2IllegalSubjectException(L"Expected "##L#_valuetype_##L" as 'this' object");	\
				return false;	\
			}	\
			_type_ *_var_ = (_type_*)&subject;


#define LS2_SUBJECT_AS_CHAR(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Char,LavishScript2::LS2CodeBoxValue_Char,_var_)
#define LS2_SUBJECT_AS_STRING(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_String,LavishScript2::LS2CodeBoxValue_String,_var_)
#define LS2_SUBJECT_AS_BOOLEAN(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Boolean,LavishScript2::LS2CodeBoxValue_Boolean,_var_)
#define LS2_SUBJECT_AS_INT8(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Int8,LavishScript2::LS2CodeBoxValue_Int8,_var_)
#define LS2_SUBJECT_AS_INT16(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Int16,LavishScript2::LS2CodeBoxValue_Int16,_var_)
#define LS2_SUBJECT_AS_INT32(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Int32,LavishScript2::LS2CodeBoxValue_Int32,_var_)
#define LS2_SUBJECT_AS_INT64(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Int64,LavishScript2::LS2CodeBoxValue_Int64,_var_)
#define LS2_SUBJECT_AS_INTPTR(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_IntPtr,LavishScript2::LS2CodeBoxValue_IntPtr,_var_)
#define LS2_SUBJECT_AS_UINT8(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_UInt8,LavishScript2::LS2CodeBoxValue_UInt8,_var_)
#define LS2_SUBJECT_AS_UINT16(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_UInt16,LavishScript2::LS2CodeBoxValue_UInt16,_var_)
#define LS2_SUBJECT_AS_UINT32(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_UInt32,LavishScript2::LS2CodeBoxValue_UInt32,_var_)
#define LS2_SUBJECT_AS_UINT64(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_UInt64,LavishScript2::LS2CodeBoxValue_UInt64,_var_)
#define LS2_SUBJECT_AS_DECIMAL(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Decimal,LavishScript2::LS2CodeBoxValue_Decimal,_var_)
#define LS2_SUBJECT_AS_DOUBLE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Double,LavishScript2::LS2CodeBoxValue_Double,_var_)
#define LS2_SUBJECT_AS_SINGLE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Single,LavishScript2::LS2CodeBoxValue_Single,_var_)
#define LS2_SUBJECT_AS_BINARY(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Binary,LavishScript2::LS2CodeBoxValue_Binary,_var_)
#define LS2_SUBJECT_AS_ARRAY(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Array,LavishScript2::LS2CodeBoxValue_Array,_var_)
#define LS2_SUBJECT_AS_TABLE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Table,LavishScript2::LS2CodeBoxValue_Table,_var_)
#define LS2_SUBJECT_AS_DELEGATE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Delegate,LavishScript2::LS2CodeBoxValue_Delegate,_var_)
#define LS2_SUBJECT_AS_OBJECT(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Object,LavishScript2::LS2CodeBoxValue_Object,_var_)
#define LS2_SUBJECT_AS_TYPE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Type,LavishScript2::LS2CodeBoxValue_Type,_var_)
//#define LS2_SUBJECT_AS_EXCEPTION(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Exception,LavishScript2::LS2CodeBoxValue_Exception,_var_)
#define LS2_SUBJECT_AS_REFERENCE(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Reference,LavishScript2::LS2CodeBoxValue_Reference,_var_)
#define LS2_SUBJECT_AS_FUNCTION(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Function,LavishScript2::LS2CodeBoxValue_Function,_var_)
#define LS2_SUBJECT_AS_METHOD(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Method,LavishScript2::LS2CodeBoxValue_Method,_var_)
#define LS2_SUBJECT_AS_STATICMETHOD(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_StaticMethod,LavishScript2::LS2CodeBoxValue_StaticMethod,_var_)
#define LS2_SUBJECT_AS_PROPERTY(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Property,LavishScript2::LS2CodeBoxValue_Property,_var_)
#define LS2_SUBJECT_AS_STATICPROPERTY(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_StaticProperty,LavishScript2::LS2CodeBoxValue_StaticProperty,_var_)
#define LS2_SUBJECT_AS_FIELD(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Field,LavishScript2::LS2CodeBoxValue_Field,_var_)
#define LS2_SUBJECT_AS_STATICFIELD(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_StaticField,LavishScript2::LS2CodeBoxValue_StaticField,_var_)
#define LS2_SUBJECT_AS_NULL(_var_)	LS2_SUBJECT_AS(LavishScript2::VT_Null,LavishScript2::LS2CodeBoxValue_Null,_var_)

#define LS2_SUBJECT_AS_OBJECT_(_objtype_,_type_,_var_)	\
		LS2_SUBJECT_AS_OBJECT(obj##_var_); \
		if (&obj##_var_->m_Type!=_objtype_) \
		{\
			*ppException = new LavishScript2::LS2IllegalSubjectException(L"Unexpected 'this' object type");\
			return false;\
		}\
		_type_ *_var_ = (_type_*)obj##_var_;

#define LS2_INPUT_AS_CHAR(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Char,LavishScript2::LS2CodeBoxValue_Char,_var_)
#define LS2_INPUT_AS_STRING(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_String,LavishScript2::LS2CodeBoxValue_String,_var_)
#define LS2_INPUT_AS_BOOLEAN(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Boolean,LavishScript2::LS2CodeBoxValue_Boolean,_var_)
#define LS2_INPUT_AS_INT8(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Int8,LavishScript2::LS2CodeBoxValue_Int8,_var_)
#define LS2_INPUT_AS_INT16(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Int16,LavishScript2::LS2CodeBoxValue_Int16,_var_)
#define LS2_INPUT_AS_INT32(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Int32,LavishScript2::LS2CodeBoxValue_Int32,_var_)
#define LS2_INPUT_AS_INT64(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Int64,LavishScript2::LS2CodeBoxValue_Int64,_var_)
#define LS2_INPUT_AS_INTPTR(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_IntPtr,LavishScript2::LS2CodeBoxValue_IntPtr,_var_)
#define LS2_INPUT_AS_UINT8(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_UInt8,LavishScript2::LS2CodeBoxValue_UInt8,_var_)
#define LS2_INPUT_AS_UINT16(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_UInt16,LavishScript2::LS2CodeBoxValue_UInt16,_var_)
#define LS2_INPUT_AS_UINT32(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_UInt32,LavishScript2::LS2CodeBoxValue_UInt32,_var_)
#define LS2_INPUT_AS_UINT64(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_UInt64,LavishScript2::LS2CodeBoxValue_UInt64,_var_)
#define LS2_INPUT_AS_DECIMAL(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Decimal,LavishScript2::LS2CodeBoxValue_Decimal,_var_)
#define LS2_INPUT_AS_DOUBLE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Double,LavishScript2::LS2CodeBoxValue_Double,_var_)
#define LS2_INPUT_AS_SINGLE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Single,LavishScript2::LS2CodeBoxValue_Single,_var_)
#define LS2_INPUT_AS_BINARY(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Binary,LavishScript2::LS2CodeBoxValue_Binary,_var_)
#define LS2_INPUT_AS_ARRAY(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Array,LavishScript2::LS2CodeBoxValue_Array,_var_)
#define LS2_INPUT_AS_TABLE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Table,LavishScript2::LS2CodeBoxValue_Table,_var_)
#define LS2_INPUT_AS_DELEGATE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Delegate,LavishScript2::LS2CodeBoxValue_Delegate,_var_)
#define LS2_INPUT_AS_OBJECT(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Object,LavishScript2::LS2CodeBoxValue_Object,_var_)
//#define LS2_INPUT_AS_EXCEPTION(_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Exception,LavishScript2::LS2CodeBoxValue_Exception,_var_)
#define LS2_INPUT_AS_TYPE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Type,LavishScript2::LS2CodeBoxValue_Type,_var_)
#define LS2_INPUT_AS_REFERENCE(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Reference,LavishScript2::LS2CodeBoxValue_Reference,_var_)
#define LS2_INPUT_AS_FUNCTION(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Function,LavishScript2::LS2CodeBoxValue_Function,_var_)
#define LS2_INPUT_AS_METHOD(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Method,LavishScript2::LS2CodeBoxValue_Method,_var_)
#define LS2_INPUT_AS_STATICMETHOD(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_StaticMethod,LavishScript2::LS2CodeBoxValue_StaticMethod,_var_)
#define LS2_INPUT_AS_PROPERTY(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Property,LavishScript2::LS2CodeBoxValue_Property,_var_)
#define LS2_INPUT_AS_STATICPROPERTY(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_StaticProperty,LavishScript2::LS2CodeBoxValue_StaticProperty,_var_)
#define LS2_INPUT_AS_FIELD(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Field,LavishScript2::LS2CodeBoxValue_Field,_var_)
#define LS2_INPUT_AS_STATICFIELD(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_StaticField,LavishScript2::LS2CodeBoxValue_StaticField,_var_)
#define LS2_INPUT_AS_NULL(_n_,_var_)	LS2_INPUT_AS(_n_,LavishScript2::VT_Null,LavishScript2::LS2CodeBoxValue_Null,_var_)



#define INPUTDECLS(...) { __VA_ARGS__ }
#define INPUTDECL(_name_,_type_,_csharp_declaration_) {_name_,_type_,_csharp_declaration_}
#define RETURNDECL(_return_type_) INPUTDECL(L"",L"LavishScript2.Reference",_return_type_)

#define REGISTER_METHOD(_type_,_name_,_cfunction_,_simple_declaration_nodes_) \
		{\
			LavishScript2::LS2SmartRef<LavishScript2::ILS2Array> pInputDeclarations;\
			LavishScript2::LS2CodeBoxInputDeclaration::simple_declaration_node inputs[] = _simple_declaration_nodes_;\
			LavishScript2::LS2CodeBoxInputDeclaration::GenerateDeclarationArray(inputs,sizeof(inputs)/sizeof(inputs[0]),pInputDeclarations);\
			_type_->RegisterMethod(_name_,pInputDeclarations,_cfunction_);\
		}

#define REGISTER_METHOD_TOSTRING(_type_,_cfunction_) REGISTER_METHOD(_type_,L"ToString{}",_cfunction_,INPUTDECLS(RETURNDECL(L"System.String")));

#define REGISTER_STATIC_METHOD(_type_,_name_,_cfunction_,_simple_declaration_nodes_) \
		{\
			LavishScript2::LS2SmartRef<LavishScript2::ILS2Array> pInputDeclarations;\
			LavishScript2::LS2CodeBoxInputDeclaration::simple_declaration_node inputs[] = _simple_declaration_nodes_;\
			LavishScript2::LS2CodeBoxInputDeclaration::GenerateDeclarationArray(inputs,sizeof(inputs)/sizeof(inputs[0]),pInputDeclarations);\
			_type_->RegisterStaticMethod(_name_,pInputDeclarations,_cfunction_);\
		}



#define LS2_AUTOPROP_GET_(_function_,_fieldid_variable_,_property_name_,_type_name_) \
	static bool __stdcall _function_(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue **ppOutput, LavishScript2::LS2Exception **ppException) \
			{ \
				LS2_SUBJECT_AS(LavishScript2::VT_LS2ManagedObject, LavishScript2::LS2CodeBoxValue_LS2ManagedObject, pObject);\
				if (_fieldid_variable_ < 0)\
				{\
					size_t outFieldID = (size_t)-1;\
					if (!pObject->m_pValue->ResolveFieldID(L"_" ## _property_name_, outFieldID, ppException))\
					{\
						(*ppException)->Tag(L"" ## _type_name_ ## L"." ## _property_name_); \
						return false;\
					}\
					_fieldid_variable_ = (int)outFieldID; \
				}\
				if (!pObject->m_pValue->GetField(_fieldid_variable_, ppOutput, ppException))\
				{\
					(*ppException)->Tag(L"" ## _type_name_ ## L"." ## _property_name_);\
					return false;\
				}\
				return true;\
			}
#define LS2_AUTOPROP_SET_(_function_,_fieldid_variable_,_property_name_,_type_name_) \
	static bool __stdcall _function_(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException) \
			{ \
				LS2_SUBJECT_AS(LavishScript2::VT_LS2ManagedObject, LavishScript2::LS2CodeBoxValue_LS2ManagedObject, pObject);\
				if (_fieldid_variable_ < 0)\
				{\
					size_t outFieldID = (size_t)-1;\
					if (!pObject->m_pValue->ResolveFieldID(L"_" ## _property_name_, outFieldID, ppException))\
					{\
						(*ppException)->Tag(L"" ## _type_name_ ## L"." ## _property_name_); \
						return false;\
					}\
					_fieldid_variable_ = (int)outFieldID; \
				}\
				if (!pObject->m_pValue->SetField(_fieldid_variable_, input, ppException))\
				{\
					(*ppException)->Tag(L"" ## _type_name_ ## L"." ## _property_name_);\
					return false;\
				}\
				return true;\
			}

#define LS2_AUTOPROP_(_get_function_,_set_function_,_fieldid_variable_,_property_name_,_type_name_) \
	LS2_AUTOPROP_GET_(_get_function_,_fieldid_variable_,_property_name_,_type_name_);\
	LS2_AUTOPROP_SET_(_set_function_,_fieldid_variable_,_property_name_,_type_name_);

#define LS2_AUTOPROP(_property_,_containing_type_name_) \
	static int _##_property_##FieldID; \
	LS2_AUTOPROP_GET_(get_##_property_, _##_property_##FieldID, L"" ## L ## #_property_, _containing_type_name_); \
	LS2_AUTOPROP_SET_(set_##_property_, _##_property_##FieldID, L"" ## L ## #_property_, _containing_type_name_);

#define LS2_AUTOPROP_GET(_property_,_containing_type_name_) \
	static int _##_property_##FieldID; \
	LS2_AUTOPROP_GET_(get_##_property_, _##_property_##FieldID, L"" ## L ## #_property_, _containing_type_name_);

#define LS2_AUTOPROP_SET(_property_,_containing_type_name_) \
	static int _##_property_##FieldID; \
	LS2_AUTOPROP_SET_(set_##_property_, _##_property_##FieldID, L"" ## L ## #_property_, _containing_type_name_);

#define LS2_AUTOPROP_FIELD(_class_,_property_) \
	int _class_::_##_property_##FieldID = -1;


/*
	static bool __stdcall set_##_property_(LavishScript2::LS2CodeBoxValue &subject, LavishScript2::LS2CodeBoxValue &input, LavishScript2::LS2Exception **ppException) \
			{ \
			LS2_SUBJECT_AS(LavishScript2::VT_LS2ManagedObject, LavishScript2::LS2CodeBoxValue_LS2ManagedObject, pObject);\
			if (_##_property_##FieldID < 0)\
				{\
				size_t outFieldID = (size_t)-1;\
				if (!pObject->m_pValue->ResolveFieldID(L"_" ## L ## #_property_), outFieldID, ppException)) \
					{\
					(*ppException)->Tag(L"" ## _containing_type_name_ ## L"." ## L ## #_property_); \
					return false;\
					}\
					_##_property_##FieldID = (int)outFieldID; \
				}\
				if (!pObject->m_pValue->SetField(_##_property_##FieldID, input, ppException))\
				{\
				(*ppException)->Tag(L"" ## _containing_type_name_ ## L"." ## L ## #_property_); \
				return false;\
				}\
				return true;\
			}
			*/

//	LS2_AUTOPROP_GET(get_##_property_, _##_property_##FieldID, L"" ## _property_, _containing_type_name_); \
//	LS2_AUTOPROP_SET(set_##_property_, _##_property_##FieldID, L"" ## _property_, _containing_type_name_);

#define REGISTER_AUTOPROP(_class_,_type_,_property_,_property_type_name_) \
	_type_->RegisterField(L"_" ## L ## # _property_, _property_type_name_, L"_" ## _property_type_name_ ## L" " ## L ## #_property_, 0, 0); \
	_type_->RegisterProperty(L ## # _property_, _property_type_name_, 0, &_class_::get_##_property_, &_class_::set_##_property_);

#define REGISTER_AUTOPROP_GET(_class_,_type_,_property_,_property_type_name_) \
	_type_->RegisterField(L"_" ## L ## # _property_, _property_type_name_, L"_" ## _property_type_name_ ## L" " ## L ## #_property_, 0, 0); \
	_type_->RegisterProperty(L ## # _property_, _property_type_name_, 0, &_class_::get_##_property_, 0);
#define REGISTER_AUTOPROP_SET(_class_,_type_,_property_,_property_type_name_) \
	_type_->RegisterField(L"_" ## L ## # _property_, _property_type_name_, L"_" ## _property_type_name_ ## L" " ## L ## #_property_, 0, 0); \
	_type_->RegisterProperty(L ## # _property_, _property_type_name_, 0, 0, &_class_::set_##_property_);


/*
#define REGISTER_AUTOPROP(_type_,_property_name_,_property_type_name_,_get_function_,_set_function_) \
	_type_->RegisterField(L"_" ## _property_name_, _property_type_name_, L"_" ## _property_type_name_ ## L" " ## _property_name_, 0, 0); \
	_type_->RegisterProperty(_property_name_, _property_type_name_, 0, _get_function_, _set_function_);
*/

	class ILS2Signal
	{
	public:
		virtual void Signal()=0;
		virtual void Reset()=0;
		virtual bool IsSignaled()=0;
	};

	// you implement this in your own ls2 module so LS2 can start and stop it.
	class ILS2Module
	{
	public:
		virtual unsigned int GetAPIVersion();

		virtual bool Initialize(LavishScript2::LS2BuildInfo &environment_version, LavishScript2::ILS2StandardEnvironment *pEnvironment, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Shutdown(LavishScript2::ILS2Signal *pShutdownComplete__null_if_too_late_for_asynchronous_shutdown, LavishScript2::LS2Exception **ppException)=0;

		// call this in your Initialize function!
		static void AttachEnvironment(LavishScript2::ILS2StandardEnvironment *pEnvironment);
	};

	typedef LavishScript2::ILS2Module * (__stdcall *fGetLS2Module)(unsigned int SDK_VERSION, const wchar_t *module_name);

	// this describes a registered module
	class ILS2AvailableModule
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual bool GetName(ILS2String **ppString)=0;
		virtual bool GetFilename(ILS2String **ppString)=0;

		virtual bool IsStarted()=0;
		virtual bool Start(LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Stop(LavishScript2::LS2Exception **ppException)=0;

		virtual bool GetMetaTable(ILS2Table **ppTable)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2AvailableModule*> LS2CodeBoxValue_Module;

	// describes a thread of an LS2 script
	class ILS2Thread
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual unsigned int GetID()=0;

		virtual bool IsStarted()=0;
		virtual bool Start(ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Stop(LavishScript2::LS2Exception **ppException)=0;

		virtual bool IsPaused()=0;
		virtual bool Pause(bool bNewState, LavishScript2::LS2Exception **ppException)=0;

		virtual bool GetMetaTable(ILS2Table **ppTable)=0;

		virtual bool GetScript(ILS2Script **ppScript)=0;

		virtual unsigned int GetMaxInstructionsPerCycle()=0;
		virtual bool SetMaxInstructionsPerCycle(unsigned int newValue, LavishScript2::LS2Exception **ppException)=0;

		// a First-Class thread will prevent the Script from terminating if it is running
		virtual bool IsFirstClass()=0;
		virtual bool SetFirstClass(bool bFirstClass, LavishScript2::LS2Exception **ppException)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2Thread*> LS2CodeBoxValue_Thread;

	// this describes a LS2 script (bytecode)
	class ILS2Script
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		virtual bool GetName(LavishScript2::ILS2String **ppString)=0;
		virtual bool GetFilename(LavishScript2::ILS2String **ppString)=0;

		virtual bool IsStarted()=0;
		virtual bool Start(ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool Stop(LavishScript2::LS2Exception **ppException)=0;

		virtual bool IsPaused()=0;
		virtual bool Pause(bool bNewState, LavishScript2::LS2Exception **ppException)=0;

		virtual bool GetMetaTable(ILS2Table **ppTable)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2Script*> LS2CodeBoxValue_Script;

	// you implement this to consume an LS2 event
	class ILS2EventTarget
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		
		virtual bool Execute(ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;
		virtual bool GetMetaTable(ILS2Table **ppTable)=0;
	};

	// this describes an LS2 event
	class ILS2Event
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;
		virtual bool GetName(LavishScript2::ILS2String **ppString)=0;

		virtual void Attach(LavishScript2::ILS2EventTarget &event_target)=0;
		virtual void Detach(LavishScript2::ILS2EventTarget &event_target)=0;

		virtual bool Execute(LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)=0;

		// this is the number of handlers attached.
		virtual size_t GetLength() = 0;
		virtual bool GetMetaTable(ILS2Table **ppTable)=0;
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2Event*> LS2CodeBoxValue_Event;
	/*
	class LS2RoutedEvent
	{
	public:
		virtual int AddRef();
		virtual int Delete();

	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::LS2RoutedEvent*> LS2CodeBoxValue_RoutedEvent;

	class LS2EventArgs
	{
	public:
		virtual int AddRef();
		virtual int Delete();

	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::LS2EventArgs*> LS2CodeBoxValue_EventArgs;

	class LS2RoutedEventArgs : public LS2EventArgs
	{
	public:
		virtual bool GetHandled();
		virtual void SetHandled(bool new_value);

		virtual void GetOriginalSource(LavishScript2::LS2CodeBoxValue **ppValue);
		virtual void SetOriginalSource(LavishScript2::LS2CodeBoxValue &value);

		virtual void GetRoutedEvent(LavishScript2::LS2RoutedEvent **ppRoutedEvent);
		virtual void SetRoutedEvent(LavishScript2::LS2RoutedEvent &routed_event);

		virtual void GetSource(LavishScript2::LS2CodeBoxValue **ppValue);
		virtual void SetSource(LavishScript2::LS2CodeBoxValue &value);
	};
	typedef LavishScript2::LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::LS2RoutedEventArgs*> LS2CodeBoxValue_RoutedEventArgs;
	/**/

	class ILS2StandardEnvironment : public LavishScript2::ILS2CodeBoxEnvironment
	{
	public:
		virtual void GetVersion(LavishScript2::LS2BuildInfo &version)=0;


		// Tell the environment about an available module (DLL)
		virtual bool RegisterModule(const wchar_t *name, const wchar_t *filename)=0;
		// Tell the environment about an available module (pre-loaded)
		virtual bool RegisterModule(const wchar_t *name, fGetLS2Module module_loader)=0;

		virtual bool UnregisterModule(const wchar_t *name, LavishScript2::LS2Exception **ppException)=0;
		// Retrieve the available module
		virtual bool GetModule(const wchar_t *name, LavishScript2::ILS2AvailableModule **ppModule)=0;
		// List the available modules
		virtual size_t EnumModules(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumModules(LavishScript2::ILS2Table &populate_table)=0;


		// Tell the environment about an available script (bytecode)
		virtual bool RegisterScript(const wchar_t *name, const wchar_t *filename)=0;

		virtual bool RegisterLS2ILScript(const wchar_t *name, const wchar_t *filename, LavishScript2::LS2Exception **ppException)=0;

		// Tell the environment about an available script (pre-loaded bytecode)
		virtual bool RegisterScript(const wchar_t *name, LS2Buffer &buffer)=0;

		virtual bool UnregisterScript(const wchar_t *name, LavishScript2::LS2Exception **ppException)=0;
		// Retrieve the available script
		virtual bool GetScript(const wchar_t *name, ILS2Script **ppScript)=0;
		// List the available scripts
		virtual size_t EnumScripts(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumScripts(LavishScript2::ILS2Table &populate_table)=0;
		
		// Tell the environment about an available event
		// Retrieve the available event
		virtual bool RegisterEvent(const wchar_t *name, LavishScript2::ILS2Event **ppEvent)=0;
		// List the available events
		virtual size_t EnumEvents(LavishScript2::fEnumCallback enum_func, void *pass_thru)=0;
		virtual size_t EnumEvents(LavishScript2::ILS2Table &populate_table)=0;

		// Constructors for built-in object types...
		virtual void NewScript(LavishScript2::ILS2Script &in_script, LavishScript2::LS2CodeBoxValue_Script **ppValue)=0;
		virtual void NewThread(LavishScript2::ILS2Thread &in_thread, LavishScript2::LS2CodeBoxValue_Thread **ppValue)=0;
		virtual void NewEvent(LavishScript2::ILS2Event &in_event, LavishScript2::LS2CodeBoxValue_Event **ppValue)=0;
		virtual void NewModule(LavishScript2::ILS2AvailableModule &in_module, LavishScript2::LS2CodeBoxValue_Module **ppValue)=0;

		bool BootStrapLS2ILResource(HMODULE hModule, const wchar_t *resourceName, const wchar_t *resourceType, LavishScript2::LS2Exception **ppException);
		bool BootStrapLS2ILFile(const wchar_t *filename, LavishScript2::LS2Exception **ppException);
		static LavishScript2::ILS2StandardEnvironment *s_pInstance;
	};



};

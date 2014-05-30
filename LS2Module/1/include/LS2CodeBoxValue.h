#pragma once
#include <crtdefs.h>
namespace LavishScript2
{
	class LS2Exception;
	class ILS2String;
	class ILS2CodeBoxManagedObject;
	class LS2Buffer;
	class ILS2CodeBoxEnvironment;
	class ILS2Array;
	class ILS2Table;
	class ILS2CodeBoxMethod;
	class ILS2CodeBoxStaticMethod;
	class ILS2CodeBoxProperty;
	class ILS2CodeBoxStaticProperty;
	class ILS2CodeBoxField;
	class ILS2CodeBoxStaticField;
	class ILS2CodeBoxDelegate;
	class ILS2CodeBoxFunction;
	class ILS2KeyValuePair;

//	class LS2CodeBoxArray;
//	class LS2CodeBoxTable;
	class LS2String;
	class LS2CodeBoxType;
	class LS2CodeBoxValue;

	enum eLS2CodeBoxValueType
	{
		VT_Null=0,

        VT_Single,
        VT_Double,
        VT_Decimal,

        VT_Boolean,

        VT_Int8,
        VT_Int16,
        VT_Int32,
        VT_Int64,
        VT_UInt8,
        VT_UInt16,
        VT_UInt32,
        VT_UInt64,

		VT_IntPtr,
		VT_UIntPtr,

		VT_Delegate,
		VT_Enum,

		VT_Char,
		VT_String,
		VT_Binary,
		VT_Array,
		VT_Table,

		VT_Reference,

		VT_Function,
		//VT_Exception, // merged into VT_LS2ManagedObject

		// object support
		VT_Object,
		VT_LS2ManagedObject, // remember: it's not .NET-managed
		VT_Type,
		VT_Method,
		VT_StaticMethod,
		VT_Property,
		VT_StaticProperty,
		VT_Field,
		VT_StaticField,
	};

	class LS2CodeBoxValue //: public ILS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue(eLS2CodeBoxValueType p_Type)
		{
			m_RefCount=1;
			m_Type=p_Type;
		}
		virtual ~LS2CodeBoxValue()
		{
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

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException)=0;

		// numeric
		virtual bool Add(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException)=0;
		virtual bool Subtract(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		// logical/boolean
		virtual bool And(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException);
		virtual bool Or(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException);
		virtual bool Not(bool &output, LS2Exception **ppException);
		virtual bool Equal(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException)=0;
		virtual bool LessThan(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException)=0;
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException)=0;

		// bitwise
		virtual bool SHL(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool SHR(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool AND(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool OR(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool NOT(LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool XOR(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool GetBool(LS2Exception **ppException)=0;
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual unsigned __int64 GetUInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException)=0;
		virtual bool GetBinary(LS2Buffer **ppBuffer, LS2Exception **ppException)=0;
		virtual bool GetType(class ILS2CodeBoxType **ppType)=0;

		virtual bool GetMetaTable(class ILS2Table **ppTable)=0;
		

		static const wchar_t *GetValueTypeName(enum eLS2CodeBoxValueType _type);
		eLS2CodeBoxValueType m_Type;
	};


	class LS2CodeBoxValue_Boolean : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Boolean(bool p_Value):LS2CodeBoxValue(VT_Boolean)
		{
			Value=p_Value;
		}

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);

		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		static ILS2String *s_pTrue;
		static ILS2String *s_pFalse;
		static ILS2String *s_pStrings[2];

		bool Value;
	};

	class LS2CodeBoxValue_Single : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Single(float p_Value,eLS2CodeBoxValueType p_Type):LS2CodeBoxValue(p_Type)
		{
			Value=p_Value;
		}
		LS2CodeBoxValue_Single(float p_Value):LS2CodeBoxValue(VT_Single)
		{
			Value=p_Value;
		}

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);

	//	virtual bool And(LS2CodeBoxValue *pValue);
	//	virtual bool Or(LS2CodeBoxValue *pValue);
	//	virtual bool Not();
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		float Value;
	};
	
	class LS2CodeBoxValue_Double : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Double(double p_Value,eLS2CodeBoxValueType p_Type):LS2CodeBoxValue(p_Type)
		{
			Value=p_Value;
		}
		LS2CodeBoxValue_Double(double p_Value):LS2CodeBoxValue(VT_Double)
		{
			Value=p_Value;
		}

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);

	//	virtual bool And(LS2CodeBoxValue *pValue);
	//	virtual bool Or(LS2CodeBoxValue *pValue);
	//	virtual bool Not();
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		double Value;
	};

	class LS2CodeBoxValue_Decimal : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Decimal(double p_Value,eLS2CodeBoxValueType p_Type):LS2CodeBoxValue(p_Type)
		{
			Value=p_Value;
		}
		LS2CodeBoxValue_Decimal(double p_Value):LS2CodeBoxValue(VT_Decimal)
		{
			Value=p_Value;
		}

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);

	//	virtual bool And(LS2CodeBoxValue *pValue);
	//	virtual bool Or(LS2CodeBoxValue *pValue);
	//	virtual bool Not();
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		double Value;
	};

	class LS2CodeBoxValue_Null : public LS2CodeBoxValue_Decimal
	{
	public:
		LS2CodeBoxValue_Null():LS2CodeBoxValue_Decimal(0,VT_Null)
		{
		}

		virtual bool Add(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
	};

	template<class T, eLS2CodeBoxValueType value_type, const wchar_t *type_name, const wchar_t *printf_format>
	class LS2CodeBoxValue_IntT : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_IntT(T p_Value):LS2CodeBoxValue(value_type)
		{
			Value=p_Value;
		}

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException)
		{
			return new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value);
		}

		virtual bool GetType(class ILS2CodeBoxType **ppType)
		{
			return ILS2CodeBoxEnvironment::s_pInstance->ResolveType(type_name,ppType);
		}
		
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value+i1);
			return true;
		}
		
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput, LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value-i1);
			return true;
		}
		
		virtual bool Divide(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			if (!i1)
			{
				*ppException = new LS2DivideByZeroException(0);
				return false;
			}
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value/i1);
			return true;
		}
		
		virtual bool Multiply(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value*i1);
			return true;
		}
		
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(Value%i1);
			return true;
		}
		
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}

		
			*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>((T)pow((double)Value,(double)i1));
			return true;
		}
		
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException)
		{
			switch(value_type)
			{
			case VT_UInt8:
				*ppOutput = new LS2CodeBoxValue_Int8(-(__int8)Value);
				return true;
			case VT_UInt16:
				*ppOutput = new LS2CodeBoxValue_Int16(-(__int16)Value);
				return true;
			case VT_UInt32:
				*ppOutput = new LS2CodeBoxValue_Int32(-(__int32)Value);
				return true;
			case VT_UInt64:
				*ppOutput = new LS2CodeBoxValue_Int64(-(__int64)Value);
				return true;
			default:
#pragma warning( push )
#pragma warning( disable : 4146 )
				// warning C4146 is resolved by the above switch for unsigned types
				*ppOutput = new LS2CodeBoxValue_IntT<T,value_type,type_name,printf_format>(-Value);
				return true;
#pragma warning( pop )
			}
		}

		
		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			output = (Value==i1);
			return true;
		}
		
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			output = (Value<i1);
			return true;
		}
		
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException)
		{
			LS2Exception *pException = 0;
			T i1 = (T)pValue->GetInt(&pException);
			if (pException)
			{
				*ppException = pException;
				return false;
			}
			output = (Value<=i1);
			return true;
		}

		
		virtual __int64 GetLength(LS2Exception **ppException) 
		{ 
			return sizeof(Value);		
		} 
		
		virtual __int64 GetInt(LS2Exception **ppException)
		{
			return Value;
		}
		
		virtual double GetDouble(LS2Exception **ppException)
		{
			return (double)Value;
		}
		
		virtual bool GetBool(LS2Exception **ppException)
		{
			return Value!=0;
		}
		
		virtual bool GetString(ILS2String **ppString, LS2Exception **ppException)
		{
			wchar_t Buffer[64];
			_snwprintf_s(Buffer,64,printf_format,Value);
			*ppString = new LS2String(Buffer);
			return true;
		}
		
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException)
		{
			*ppBuffer = new LS2Buffer(&Value,sizeof(Value));
			return true;
		}

		virtual bool GetMetaTable(class ILS2Table **ppTable)
		{
			return false;
		}

		T Value;
	};
	
#ifndef LS2_INTEGER_TYPES
#define LS2_INTEGER_TYPES

	extern wchar_t _uint8_type[];// =L"System.Byte";
	extern wchar_t _uint8_format[];// =L"%u";
	extern wchar_t _int8_type[];// =L"System.SByte";
	extern wchar_t _int8_format[];// =L"%d";

	extern wchar_t _uint16_type[];// =L"System.UInt16";
	extern wchar_t _uint16_format[];// =L"%u";
	extern wchar_t _int16_type[];// =L"System.Int16";
	extern wchar_t _int16_format[];// =L"%d";

	extern wchar_t _uint32_type[];// =L"System.UInt32";
	extern wchar_t _uint32_format[];// =L"%I32u";
	extern wchar_t _int32_type[];// =L"System.Int32";
	extern wchar_t _int32_format[];// =L"%I32d";

	extern wchar_t _uint64_type[];// =L"System.UInt64";
	extern wchar_t _uint64_format[];// =L"%I64u";
	extern wchar_t _int64_type[];// =L"System.Int64";
	extern wchar_t _int64_format[];// =L"%I64d";

	typedef LS2CodeBoxValue_IntT<__int8,VT_Int8,_int8_type,_int8_format> LS2CodeBoxValue_Int8;
	typedef LS2CodeBoxValue_IntT<unsigned __int8,VT_UInt8,_uint8_type,_uint8_format> LS2CodeBoxValue_UInt8;

	typedef LS2CodeBoxValue_IntT<__int16,VT_Int16,_int16_type,_int16_format> LS2CodeBoxValue_Int16;
	typedef LS2CodeBoxValue_IntT<unsigned __int16,VT_UInt16,_uint16_type,_uint16_format> LS2CodeBoxValue_UInt16;

	typedef LS2CodeBoxValue_IntT<__int32,VT_Int32,_int32_type,_int32_format> LS2CodeBoxValue_Int32;
	typedef LS2CodeBoxValue_IntT<unsigned __int32,VT_UInt32,_uint32_type,_uint32_format> LS2CodeBoxValue_UInt32;

	typedef LS2CodeBoxValue_IntT<__int64,VT_Int64,_int64_type,_int64_format> LS2CodeBoxValue_Int64;
	typedef LS2CodeBoxValue_IntT<unsigned __int64,VT_UInt64,_uint64_type,_uint64_format> LS2CodeBoxValue_UInt64;
#endif

	class LS2CodeBoxValue_IntPtr : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_IntPtr(const int &value);
		LS2CodeBoxValue_IntPtr(void *value);
#ifdef _WIN64
		LS2CodeBoxValue_IntPtr(const __int64 &value);
#endif

		static bool New(int &value, LS2CodeBoxValue_IntPtr **ppOutput, LS2Exception **ppException);
		static bool New(__int64 &value, LS2CodeBoxValue_IntPtr **ppOutput, LS2Exception **ppException);
		static bool New(void *value, LS2CodeBoxValue_IntPtr **ppOutput, LS2Exception **ppException);

		virtual ~LS2CodeBoxValue_IntPtr();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		// numeric
		virtual bool Add(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Negate(LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		// logical/boolean
		virtual bool Equal(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue, bool &output, LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString, LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer, LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		void* Value;
	};

	class LS2CodeBoxValue_Enum : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Enum(ILS2CodeBoxType &enum_type, __int64 value);
		virtual ~LS2CodeBoxValue_Enum();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		// numeric
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Divide(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Multiply(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Modulo(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Power(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Negate(LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		// logical/boolean
		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		ILS2CodeBoxType *m_pEnumType;
		__int64 Value;
	};

	class LS2CodeBoxValue_Char : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Char(wchar_t value);

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		wchar_t Value;
	};

	class LS2CodeBoxValue_String : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_String(ILS2String *p_Value);
		LS2CodeBoxValue_String(const wchar_t *p_Value);/*:LS2CodeBoxValue(VT_String)
		{
			Value=p_Value;
		}*/

		LS2CodeBoxValue_String(const wchar_t *p_Value, size_t part_length);
		virtual ~LS2CodeBoxValue_String();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);
//		virtual bool Subtract(LS2CodeBoxValue *pValue,LS2Exception **ppException);
		//virtual bool Divide(LS2CodeBoxValue *pValue,LS2Exception **ppException);
		//virtual bool Multiply(LS2CodeBoxValue *pValue,LS2Exception **ppException);

	//	virtual bool And(LS2CodeBoxValue *pValue);
	//	virtual bool Or(LS2CodeBoxValue *pValue);
	//	virtual bool Not();
	//	virtual bool Negate(LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		ILS2String *m_pValue;
	};



	//
	class LS2CodeBoxValue_Binary : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Binary(class LS2Buffer *pBuffer);
		virtual ~LS2CodeBoxValue_Binary();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual __int64 GetInt(LS2Exception **ppException);
		virtual double GetDouble(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);

		LS2Buffer *m_pBuffer;
	};


	class LS2CodeBoxValue_Array : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Array();
		LS2CodeBoxValue_Array(LavishScript2::ILS2Array *pArray);
		virtual ~LS2CodeBoxValue_Array();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);


		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType); 

		virtual bool GetMetaTable(class ILS2Table **ppTable);

		LavishScript2::ILS2Array *m_pArray;
	};

	class LS2CodeBoxValue_Table : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Table();
		LS2CodeBoxValue_Table(LavishScript2::ILS2Table *pTable);
		virtual ~LS2CodeBoxValue_Table();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType); 

		virtual bool GetMetaTable(class ILS2Table **ppTable);

		LavishScript2::ILS2Table *m_pTable;		
	};

	template<class T>
	class LS2CodeBoxPointer : public LS2CodeBoxValue
	{
	public:
		/*
		LS2CodeBoxPointer(eLS2CodeBoxValueType p_Type, T *pValue);
		~LS2CodeBoxPointer();
		/**/
		LS2CodeBoxPointer(eLS2CodeBoxValueType p_Type, T *pValue):LS2CodeBoxValue(p_Type)
		{
			if (pValue)
				pValue->AddRef();
			m_pValue = pValue;
		}
		virtual ~LS2CodeBoxPointer()
		{
			if (m_pValue)
			{
				m_pValue->Delete();
				m_pValue=0;
			}
		}

		virtual bool GetBinary(LS2Buffer **ppBuffer, LS2Exception **ppException)
		{
		
			return false;
		}
		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException)
		{
			*ppException = new LS2IllegalOperandException(L"LS2CodeBoxPointer<T> Duplicate");
			return 0;
		}

		virtual bool Add(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput,LS2Exception **ppException)
		{
			*ppException = new LS2IllegalOperandException(L"LS2CodeBoxPointer<T> Add");
			return false;
		}
		virtual bool Equal(LS2CodeBoxValue *pValue, bool &output,LS2Exception **ppException)
		{
			if (m_Type!=pValue->m_Type)
			{
				output = false;
				return true;
			}

			LS2CodeBoxPointer<T> *pOther = (LS2CodeBoxPointer<T> *)pValue;
			output = (this->m_pValue == pOther->m_pValue);
			return true;
		}
		virtual bool LessThan(LS2CodeBoxValue *pValue, bool &output,LS2Exception **ppException)
		{
			*ppException = new LS2IllegalOperandException(L"LS2CodeBoxPointer<T> LessThan");
			return false;		
		}
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue, bool &output,LS2Exception **ppException)
		{
			*ppException = new LS2IllegalOperandException(L"LS2CodeBoxPointer<T> LessThanEqual");
			return false;
		}

		virtual bool GetBool(LS2Exception **ppException)
		{
			return m_pValue!=0;
		}

		T *m_pValue;
	};

	class LS2CodeBoxValue_Delegate : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxDelegate>
	{
	public:
		LS2CodeBoxValue_Delegate(LavishScript2::ILS2CodeBoxDelegate &p_delegate);
		virtual ~LS2CodeBoxValue_Delegate();

		virtual bool GetString(ILS2String **ppString, LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);

		virtual bool GetMetaTable(class ILS2Table **ppTable);

		virtual bool Add(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);
		virtual bool Subtract(LS2CodeBoxValue *pValue, LS2CodeBoxValue **ppOutput, LS2Exception **ppException);


//		class ILS2Table *m_pMetaTable;
	};

	class LS2CodeBoxValue_Reference : public LS2CodeBoxPointer<LavishScript2::LS2CodeBoxValue>
	{
	public:
		LS2CodeBoxValue_Reference(LS2CodeBoxValue *pValue);
		virtual ~LS2CodeBoxValue_Reference();

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);

		virtual void SetReference(LS2CodeBoxValue *pNewValue, bool bAddRef=true);
		virtual bool GetMetaTable(class ILS2Table **ppTable);


		class ILS2Table *m_pMetaTable;
	};


	class LS2CodeBoxValue_Object : public LS2CodeBoxValue
	{
	public:
		LS2CodeBoxValue_Object(class ILS2CodeBoxType &_Type);
		virtual ~LS2CodeBoxValue_Object();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);
		virtual bool Add(LS2CodeBoxValue *pValue,LS2CodeBoxValue **ppOutput,LS2Exception **ppException);

		virtual bool Equal(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThan(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);
		virtual bool LessThanEqual(LS2CodeBoxValue *pValue,bool &output,LS2Exception **ppException);

		virtual __int64 GetLength(LS2Exception **ppException);
		virtual bool GetBool(LS2Exception **ppException);
		virtual bool GetBinary(LS2Buffer **ppBuffer,LS2Exception **ppException);

		virtual bool GetMetaTable(class ILS2Table **ppTable);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);

		class ILS2CodeBoxType &m_Type;
	};
	
	template<class T>
	class LS2CodeBoxValue_ObjectT : public LS2CodeBoxValue_Object
	{
	public:
		LS2CodeBoxValue_ObjectT(class ILS2CodeBoxType &_Type, T _Object) : LS2CodeBoxValue_Object(_Type),m_Object(_Object)
		{
			
		}

		virtual ~LS2CodeBoxValue_ObjectT()
		{
		}

		T m_Object;
	};

	template<class T>
	class LS2CodeBoxValue_ObjectRefCountedT : public LS2CodeBoxValue_ObjectT<T>
	{
	public:
		LS2CodeBoxValue_ObjectRefCountedT(class ILS2CodeBoxType &_Type, T _Object) : LS2CodeBoxValue_ObjectT(_Type,_Object)
		{
			if (_Object)
				_Object->AddRef();
		}
		virtual ~LS2CodeBoxValue_ObjectRefCountedT()
		{
			if (m_Object)
			{
				m_Object->Delete();
				m_Object=0;
			}
		}
	};
	

	class LS2CodeBoxValue_LS2ManagedObject : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxManagedObject>
	{
	public:
		LS2CodeBoxValue_LS2ManagedObject(LavishScript2::ILS2CodeBoxManagedObject *pObject);
//		virtual ~LS2CodeBoxValue_LS2ManagedObject();

		virtual LS2CodeBoxValue *Duplicate(LS2Exception **ppException);

		virtual bool GetMetaTable(class ILS2Table **ppTable);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);

	};

	class LS2CodeBoxValue_Function : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxFunction>
	{
	public:
		LS2CodeBoxValue_Function(LavishScript2::ILS2CodeBoxFunction *pFunction);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_Type : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxType>
	{
	public:
		LS2CodeBoxValue_Type(ILS2CodeBoxType *pType);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_Method : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxMethod>
	{
	public:
		LS2CodeBoxValue_Method(ILS2CodeBoxMethod *pMethod);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_StaticMethod : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxStaticMethod>
	{
	public:
		LS2CodeBoxValue_StaticMethod(ILS2CodeBoxStaticMethod *pStaticMethod);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_Property : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxProperty>
	{
	public:
		LS2CodeBoxValue_Property(ILS2CodeBoxProperty *pProperty);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_StaticProperty : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxStaticProperty>
	{
	public:
		LS2CodeBoxValue_StaticProperty(ILS2CodeBoxStaticProperty *pStaticProperty);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_Field : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxField>
	{
	public:
		LS2CodeBoxValue_Field(ILS2CodeBoxField *pField);
		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	class LS2CodeBoxValue_StaticField : public LS2CodeBoxPointer<LavishScript2::ILS2CodeBoxStaticField>
	{
	public:
		LS2CodeBoxValue_StaticField(ILS2CodeBoxStaticField *pStaticField);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	/*
	class LS2CodeBoxValue_Exception : public LS2CodeBoxPointer<LavishScript2::LS2Exception>
	{
	public:
		LS2CodeBoxValue_Exception(LavishScript2::LS2Exception *pException);

		virtual bool GetString(ILS2String **ppString,LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};

	/*
	class LS2CodeBoxValue_KeyValuePair : public LS2CodeBoxValue_ObjectRefCountedT<LavishScript2::ILS2KeyValuePair*>
	{
	public:		
		LS2CodeBoxValue_KeyValuePair(LavishScript2::ILS2KeyValuePair *pKeyValuePair);
		virtual bool GetString(ILS2String **ppString,LavishScript2::LS2Exception **ppException);
		virtual bool GetType(class ILS2CodeBoxType **ppType);
		virtual bool GetMetaTable(class ILS2Table **ppTable);
	};
	/**/
};

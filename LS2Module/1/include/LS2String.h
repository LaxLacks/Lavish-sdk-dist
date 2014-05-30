#pragma once
#include <string>

namespace LavishScript2
{
	class ILS2String8;
	class LS2Exception;

	class ILS2String
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		// retrieve the length of the string
		virtual size_t GetLength() = 0;
		// 
		virtual const wchar_t *c_str()=0;

		// concatenate... string=string+more
		virtual void Append(const wchar_t *text)=0;
		// concatenate... string=string+more
		virtual void Append(const wchar_t *text, size_t part_length) = 0;

		virtual void Assign(const wchar_t *text)=0;

		virtual void Assign(const wchar_t *text, size_t part_length) = 0;

		virtual bool GetLS2String8(LavishScript2::ILS2String8 **ppOutput, LavishScript2::LS2Exception **ppException)=0;
	};

	class LS2String : public ILS2String
	{
	public:
		LS2String();

		LS2String(const wchar_t *text);
		LS2String(const wchar_t *text, size_t part_length);

		// string will be formatted into a temporary buffer fitting this many wchars..
		LS2String(size_t buffer_wchars, const wchar_t *szFormat, ...);

		static bool Format(ILS2String **ppString, const wchar_t *szFormat, ...);

		~LS2String(void);

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

		// retrieve the length of the string
		virtual size_t GetLength();
		// 
		virtual const wchar_t *c_str();

		// concatenate... string=string+more
		virtual void Append(const wchar_t *text);
		// concatenate... string=string+more
		virtual void Append(const wchar_t *text, size_t part_length);

		virtual void Assign(const wchar_t *text);

		virtual void Assign(const wchar_t *text, size_t part_length);

		virtual bool GetLS2String8(LavishScript2::ILS2String8 **ppOutput, LavishScript2::LS2Exception **ppException);

		std::wstring m_Value;
	};

	class ILS2String8
	{
	public:
		virtual int AddRef()=0;
		virtual int Delete()=0;

		// retrieve the length of the string
		virtual size_t GetLength() = 0;
		// 
		virtual const char *c_str()=0;

		// concatenate... string=string+more
		virtual void Append(const char *text)=0;
		// concatenate... string=string+more
		virtual void Append(const char *text, size_t part_length) = 0;

		virtual void Assign(unsigned int code_page,const char *text)=0;

		virtual void Assign(unsigned int code_page, const char *text, size_t part_length) = 0;

		virtual bool GetLS2String(LavishScript2::ILS2String **ppOutput, LavishScript2::LS2Exception **ppException)=0;
	};

	class LS2String8 : public ILS2String8
	{
	public:
		LS2String8(unsigned int code_page);

		LS2String8(unsigned int code_page,const char *text);
		LS2String8(unsigned int code_page, const char *text, size_t part_length);

		// string will be formatted into a temporary buffer fitting this many chars..
		LS2String8(unsigned int code_page,unsigned int buffer_chars,const char *szFormat, ...);

		~LS2String8(void);

		int m_RefCount;
		unsigned int m_CodePage;

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

		// retrieve the length of the string
		virtual size_t GetLength();
		// 
		virtual const char *c_str();

		// concatenate... string=string+more
		virtual void Append(const char *text);
		// concatenate... string=string+more
		virtual void Append(const char *text, size_t part_length);

		virtual void Assign(unsigned int code_page,const char *text);

		virtual void Assign(unsigned int code_page, const char *text, size_t part_length);

		virtual bool GetLS2String(LavishScript2::ILS2String **ppOutput, LavishScript2::LS2Exception **ppException);

		std::string m_Value;
	};
};

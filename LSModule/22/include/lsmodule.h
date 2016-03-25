#pragma once
#define LSMODULEVERSION 0x0022
#pragma warning( push )
#pragma warning( disable : 4996 )
#include <windows.h>
#include <map>
#include <string>
#include <utf8string.h>
#include <algorithm>
#include <math.h>
#include "WinThreading.h"
#include "Index.h"
using namespace std;
#define MakeLower(yourstring) transform (yourstring.begin(),yourstring.end(), yourstring.begin(), tolower);

#define MAX_VARSTRING 4096
#ifndef LAVISHSCRIPT
#include "LSType.h"
#include "LSSTLTypes.h"
#endif

// from Blech.h
#ifndef BLECHVERSION
typedef struct _BLECHVALUE {
	char * Name;
	char * Value;
	struct _BLECHVALUE *pNext;
} BLECHVALUE, *PBLECHVALUE;

typedef unsigned long   (__stdcall *fBlechVariableValue)(char *VarName, char *Value);
typedef void (__stdcall *fBlechCallback)(unsigned long ID, void *pData, PBLECHVALUE pValues);
#endif

typedef bool (__cdecl *fLSTopLevelObject)(int argc, char *argv[], LSOBJECT &);
typedef int (__cdecl *fLSCommand)(int argc, char *argv[]);
typedef void (__cdecl *fLSGenericEnumCallback)(const char *Name, void *pData);
typedef void (__cdecl *fLSEventTarget)(int argc, char *argv[], PLSOBJECT);

#ifndef FOPEN_UTF8
#define FOPEN_UTF8
static inline FILE *fopen_utf8(const char *filename, const char *mode)
{
	WCHAR wFilename[512];
	wFilename[0]=0;
	MultiByteToWideChar(CP_UTF8,0,filename,-1,wFilename,512);
	wFilename[511]=0;
	WCHAR wMode[512];
#pragma warning( push )
#pragma warning( disable : 4996 )
		swprintf(wMode,L"%S",mode);
	return _wfopen(wFilename,wMode);
#pragma warning( pop )
}
#endif

#ifndef FEXISTS_UTF8
#define FEXISTS_UTF8
static inline bool fexists_utf8(const char *filename)
{
	WCHAR wFilename[512];
	wFilename[0]=0;
	MultiByteToWideChar(CP_UTF8,0,filename,-1,wFilename,512);
	wFilename[511]=0;
	return (_waccess( wFilename, 0 )) != -1;
}
#endif

#ifndef WFEXISTS
#define WFEXISTS
static inline bool _wfexists(const wchar_t *wFilename)
{
	return (_waccess( wFilename, 0 )) != -1;
}
#endif

class LSInterface
{
public:


	virtual unsigned int GetVersion()=0;

	virtual bool AddModule(char *Name)=0;
	virtual bool RemoveModule(char *Name)=0;
	virtual bool IsModuleLoaded(char *Name)=0;

	// paths
	virtual char *GetLavishScriptPath(char *buffer, size_t buflen)=0;
	virtual char *GetCWD(char *buffer, size_t buflen)=0;
	virtual char *SetRoot(char *Filename, char *Dest)=0;

	virtual bool AddCommand(char *Command, fLSCommand Function, bool Parse=true, bool Hidden=false)=0;
	virtual bool RemoveCommand(char *Command)=0;
	virtual bool AddAlias(char *Replace, char *With)=0;
	virtual bool RemoveAlias(char *Replace)=0;
	virtual char *GetArgs(int BeginInclusive, int EndExclusive, char *argv[], char *buf, size_t buflen)=0;
	virtual char *GetArgsQuoted(int BeginInclusive, int EndExclusive, char *argv[], char *buf, size_t buflen)=0;
	virtual char *GetArgsQuotedNF(int BeginInclusive, int EndExclusive, char *argv[], char *buf, size_t buflen)=0;
	virtual void ExecuteTimedCommand(unsigned int DelayMS, const char *Command)=0;
	virtual int ExecuteCommand(const char *Text)=0;
	virtual int DataParse(const char *ToParse, char *buf, size_t buflen)=0;
	virtual bool DataParse(const char *Text, LSOBJECT &Object)=0;
	virtual bool RunScript(char *Filename)=0;
	virtual bool EndScript(char *Filename)=0;
	virtual bool RunCommandFile(char *Filename)=0;
	virtual bool AddTopLevelObject(char *Name, fLSTopLevelObject Function)=0;
	virtual bool RemoveTopLevelObject(char *Name)=0;
	virtual bool AddLSTypeDefinition(class LSTypeDefinition &Type)=0;
	virtual bool RemoveLSTypeDefinition(class LSTypeDefinition &Type)=0;
	virtual LSTypeDefinition *FindLSTypeDefinition(const char *Name, char *subtypebuf=0, size_t subtypebuflen=0)=0;
	virtual bool IsCommand(char *Name)=0;
	virtual bool IsAlias(char *Name)=0;
	virtual bool IsTopLevelObject(char *Name)=0;
	virtual bool IsLSTypeDefinition(char *Name)=0;
	virtual bool ResolveCommand(char *Text, char *buf,size_t buflen)=0;
	virtual void *GetTempBuffer(size_t Size, void *CopyFrom=0)=0;
	virtual void AddTempObject(CTempObject *pObject)=0;
	virtual bool RunScriptFromBuffer(char *Name, char *Buffer, size_t buflen, int argc=0, char *argv[]=0, char *CWD=0)=0;

	virtual unsigned int AddGlobalAtom(char *Buffer, size_t buflen)=0;
	virtual bool RemoveAtom(unsigned int AtomID)=0;
	virtual int ExecuteAtom(unsigned int AtomID,int argc=0, char *argv[]=0, PLSOBJECT pThisLSObject=0, PLSOBJECT pContext=0, char *ReturnBuffer=0, size_t buflen=0)=0;
	virtual int ExecuteAtom(char *Name,int argc=0, char *argv[]=0, PLSOBJECT pContext=0, char *ReturnBuffer=0, size_t buflen=0)=0;
	virtual int ExecuteAtom(char *ScriptName, char *Name,int argc=0, char *argv[]=0, PLSOBJECT pContext=0, char *ReturnBuffer=0, size_t buflen=0)=0;
	virtual unsigned int ResolveAtom(char *Name)=0;

	// enumerations
	virtual unsigned int EnumAliases(fLSGenericEnumCallback Callback, void *pData)=0;
	virtual unsigned int EnumCommands(fLSGenericEnumCallback Callback, void *pData)=0;
	virtual unsigned int EnumLSTypeDefinitions(fLSGenericEnumCallback Callback, void *pData)=0;
	virtual unsigned int EnumTopLevelObjects(fLSGenericEnumCallback Callback, void *pData)=0;
	virtual unsigned int EnumScripts(fLSGenericEnumCallback Callback, void *pData)=0;

	// scripts
	virtual bool GetCurrentScript(char *buffer, size_t buflen)=0;
	virtual bool GetScriptCWD(char *ScriptName, char *buffer, size_t buflen)=0;
	virtual bool QueueCommand(char *ScriptName, char *Command)=0;
	virtual unsigned int GetScriptRuntime(char *ScriptName)=0;
	virtual unsigned int PauseScripts()=0;
	virtual unsigned int ResumeScripts()=0;
	virtual unsigned int AddAtom(char *ScriptName, char *Buffer, size_t buflen)=0;
	
	// events
	virtual unsigned int RegisterEvent(const char *Name)=0;
	virtual void UnregisterEvent(unsigned int EventID)=0;
	virtual bool ExecuteEvent(unsigned int EventID,int BeginInclusive=0, int EndExclusive=0, char *argv[]=0, PLSOBJECT pThisLSObject=0)=0;
	virtual bool AttachEventTarget(unsigned int EventID,unsigned int AtomID)=0;
	virtual bool AttachEventTarget(unsigned int EventID,fLSEventTarget fCallback)=0;
	virtual bool DetachEventTarget(unsigned int EventID,unsigned int AtomID)=0;
	virtual bool DetachEventTarget(unsigned int EventID,fLSEventTarget fCallback)=0;

	// misc
	virtual unsigned int GetRuntime()=0;
	virtual char *Escape(char *text, char *buf, size_t buflen)=0;
	virtual char *EscapeQuotes(char *text, char *buf, size_t buflen)=0;
	virtual bool CalculateResult(const char *Formula, double &Result)=0;
	virtual bool CalculateResult(const char *Formula, __int64 &Result)=0;

	virtual void Printf(const char *Format, ...)=0;
	virtual unsigned int GetTerminalWidth()=0;

	virtual bool AddPreprocessorDefinition(char *Replace, char *With)=0;

	// compiled text and data sequences (improve performance of reused text)
	virtual unsigned int PreParseText(const char *Text, bool bDataSequence=false)=0;
	virtual bool RetrieveOriginalText(unsigned int ID, char *buf, size_t buflen)=0;
	virtual bool RetrieveProcessedText(unsigned int ID, char *buf, size_t buflen)=0;
	virtual bool RetrieveProcessedResult(unsigned int ID, LSOBJECT &Result)=0; // data sequence only
	virtual bool FreePreParsed(unsigned int ID)=0;

/* Triggers */
	virtual unsigned int AddTrigger(const char *Text, fBlechCallback Callback, void *pData)=0;
	virtual bool RemoveTrigger(unsigned int TriggerID)=0;
	virtual unsigned int CheckTriggers(const char *Text)=0;
	virtual const char *GetTriggeredText()=0;

//	virtual void LSInterface_00_Reserved() {}
//	virtual void LSInterface_01_Reserved() {}
//	virtual void LSInterface_02_Reserved() {}
//	virtual void LSInterface_03_Reserved() {}


	virtual unsigned int RegisterPersistentClass(const char *Name)=0;
	virtual void InvalidatePersistentClass(unsigned int Class)=0;
	virtual void InvalidatePersistentObject(unsigned int Class, unsigned __int64 Value) = 0;
	virtual void SetPersistentClass(LSTypeDefinition *pType,unsigned int Class)=0;


	virtual unsigned int CreateQuery(const char *Text)=0;
	virtual bool RetrieveQueryExpression(unsigned int ID,char *buf, size_t buflen)=0;
	virtual bool QueryEvaluate(unsigned int ID,LSOBJECT &Object, bool &bResult)=0;
	virtual bool FreeQuery(unsigned int ID)=0;

	virtual unsigned int PersistObject(LSOBJECT &object)=0;
	virtual bool GetPersistedObject(unsigned int key, LSOBJECT &out_object)=0;

	virtual void LSInterface_14_Reserved() {}
	virtual void LSInterface_15_Reserved() {}

// inline functions
	inline bool AddLSType(class LSTypeDefinition &Type) { return AddLSTypeDefinition(Type); }
	inline bool RemoveLSType(class LSTypeDefinition &Type) { return RemoveLSTypeDefinition(Type); }
	inline LSTypeDefinition *FindLSType(char *Name) { return FindLSTypeDefinition(Name); }

	inline bool IsLSType(char *Name) {	return IsLSTypeDefinition(Name); }
};

class LSMPreParse
{
public:
	inline LSMPreParse(class LSInterface *pInterface, const char *Text)
	{
		ID=pInterface->PreParseText(Text);
	}
	inline ~LSMPreParse()
	{
		pInterface->FreePreParsed(ID);
	}

	inline bool RetrieveOriginalText(char *buf, size_t buflen)
	{
		return pInterface->RetrieveOriginalText(ID,buf,buflen);
	}
	inline bool RetrieveProcessedText(char *buf, size_t buflen)
	{
		return pInterface->RetrieveProcessedText(ID,buf,buflen);
	}

	unsigned int ID;
	LSInterface *pInterface;
};

class LSMDataSequence
{
public:
	inline LSMDataSequence(class LSInterface *pInterface, const char *DataSequence)
	{
		ID=pInterface->PreParseText(DataSequence,true);
	}
	inline ~LSMDataSequence()
	{
		pInterface->FreePreParsed(ID);
	}

	inline bool RetrieveOriginalText(char *buf, size_t buflen)
	{
		return pInterface->RetrieveOriginalText(ID,buf,buflen);
	}
	inline bool RetrieveProcessedText(char *buf, size_t buflen)
	{
		return pInterface->RetrieveProcessedText(ID,buf,buflen);
	}
	inline bool RetrieveProcessedResult(LSOBJECT &Result)
	{
		return pInterface->RetrieveProcessedResult(ID,Result);
	}

	unsigned int ID;
	LSInterface *pInterface;
};

class LSModuleInterface
{
public:
	virtual unsigned int GetVersion() {return LSMODULEVERSION;}

	virtual bool Initialize(LSInterface *pLSInterface)=0;
	virtual void Shutdown()=0;

	virtual void Pulse() {}
	virtual void AddPreprocessorDefinitions() {}
};

#define LSModulePreSetup(_name_,_class_) \
	char ModulePath[MAX_PATH] = { 0 }; \
	char PluginLog[MAX_PATH] = { 0 }; \
	char INIFileName[MAX_PATH] = { 0 }; \
	_class_ *pModule = 0; \
	BOOL APIENTRY DllMain(HANDLE hModule, \
	DWORD  ul_reason_for_call, \
	LPVOID lpReserved \
	) \
{ \
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) \
	{ \
		GetModuleFileName((HMODULE)hModule, ModulePath, sizeof(ModulePath)); \
		if (char *pSlash = strrchr(ModulePath, '\\')) \
		{ \
			*pSlash = 0; \
		}\
		else \
		{\
			ModulePath[0] = '.'; \
			ModulePath[1] = 0; \
		}\
		sprintf(INIFileName, "%s\\%s.ini", ModulePath, _name_); \
		sprintf(PluginLog, "%s\\%s.log", ModulePath, _name_); \
	}\
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)\
	{\
		if (pModule)\
		{\
			delete pModule; \
			pModule = 0; \
		}\
	}\
	return TRUE; \
}\
\
extern "C" __declspec(dllexport) LSModuleInterface * __cdecl CreateLSModuleInterface(unsigned int LSInterfaceVersion) \
{\
if (pModule || LSInterfaceVersion != LSMODULEVERSION) \
	return 0; \
	pModule = new _class_; \
	return pModule; \
}\
extern "C" __declspec(dllexport) unsigned int LSModule_Version = LSMODULEVERSION; \
class LSTypeDefinition * FindLSTypeDefinition(const char *Name)\
{\
	return pLSInterface->FindLSTypeDefinition(Name); \
}\
\
void *GetTempBuffer(size_t Size, const void *Copy)\
{\
	return pLSInterface->GetTempBuffer(Size, (void*)Copy); \
}\
void RegisterTemporaryObject(class CTempObject *pObject)\
{\
	pLSInterface->AddTempObject(pObject); \
}\
void InvalidatePersistentObject(unsigned int persistentClass, unsigned __int64 persistedValue)\
{\
	pLSInterface->InvalidatePersistentObject(persistentClass, persistedValue);\
}\
unsigned int RegisterPersistentClass(const char *name)\
{\
	return pLSInterface->RegisterPersistentClass(name);\
}

#ifdef LSM_EXPORTS
extern LSInterface *pLSInterface;
class LSEventHelper
{
public:
	LSEventHelper(unsigned int p_EventID)
	{
		EventID=p_EventID;
		Allocated=0;
		argc=0;
		argv=0;
	}
	~LSEventHelper()
	{
		Clear();
	}

	/* Clear arguments */
	void Clear()
	{
		if (argv)
		{
			for (int i=0; i < argc ; i++)
			{
				free(argv[i]);
			}
			free(argv);

			Allocated=0;
			argv=0;
			argc=0;
		}
	}

	/* Execute without an object context */
	inline void Execute()
	{
		pLSInterface->ExecuteEvent(EventID,0,argc,argv);
	}

	/* Execute with an object context */
	inline void ExecuteWithContext(LSOBJECT &Context)
	{
		pLSInterface->ExecuteEvent(EventID,0,argc,argv,&Context);
	}

	/* Main argument adding function */
	void AddArg(const char *Text)
	{
		if (argc>=Allocated)
		{
			// allocate room for plenty of args. better once than 10 times, and windows has
			// 4096 byte granularity anyway
			
			Allocated+=20;
			argv=(char**)realloc(argv,sizeof(char**)*Allocated);
		}

		argv[argc]=strdup(Text);

		// increment count
		argc++;
	}

	/* Helpers for adding other datatypes */
	void AddArg(unsigned int &Val)
	{
		char Temp[16];// big enough for any 32-bit integer (max=4294967295)
		sprintf(Temp,"%u",Val);
		AddArg(Temp);
	}

	void AddArg(unsigned short &Val)
	{
		char Temp[8];// big enough for any 16-bit integer (max=65535)
		sprintf(Temp,"%u",Val);
		AddArg(Temp);
	}

	void AddArg(unsigned char &Val)
	{
		char Temp[8];// big enough for any 8-bit integer (max=255)
		sprintf(Temp,"%u",Val);
		AddArg(Temp);
	}

	void AddArg(unsigned __int64 &Val)
	{
		char Temp[24];// big enough for any 64-bit integer (max=18446744073709551615)
		sprintf(Temp,"%I64u",Val);
		AddArg(Temp);
	}

	void AddArg(signed int &Val)
	{
		char Temp[16];// big enough for any signed 32-bit integer
		sprintf(Temp,"%d",Val);
		AddArg(Temp);
	}

	void AddArg(signed short &Val)
	{
		char Temp[8];// big enough for any signed 16-bit integer
		sprintf(Temp,"%d",Val);
		AddArg(Temp);
	}

	void AddArg(signed char &Val)
	{
		char Temp[8];// big enough for any signed 8-bit integer 
		sprintf(Temp,"%d",Val);
		AddArg(Temp);
	}

	void AddArg(signed __int64 &Val)
	{
		char Temp[24];// big enough for any signed 64-bit integer
		sprintf(Temp,"%I64d",Val);
		AddArg(Temp);
	}

	void AddArg(float &Val)
	{
		char Temp[128];
		sprintf(Temp,"%f",Val);
		AddArg(Temp);
	}

	void AddArg(double &Val)
	{
		char Temp[128];
		sprintf(Temp,"%f",Val);
		AddArg(Temp);
	}

	void AddArg32(unsigned int Val)
	{
		char Temp[16];
		sprintf(Temp,"%u",Val);
		AddArg(Temp);
	}

	void AddArg(bool &Val)
	{
		char Temp[4];
		sprintf(Temp,"%d",Val);
		AddArg(Temp);
	}

protected:


	unsigned int EventID;
	int argc;
	char **argv;
	int Allocated;

};
#endif

#pragma warning( pop )

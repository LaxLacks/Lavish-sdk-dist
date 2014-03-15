#pragma once
#include <LSModule.h>
#include <windows.h>


class LSMFullTemplate :
	public LSModuleInterface
{
public:
	LSMFullTemplate(void);
	~LSMFullTemplate(void);

	virtual bool Initialize(LSInterface *p_LSInterface);
	virtual void Shutdown();

	void RegisterCommands();
	void RegisterAliases();
	void RegisterDataTypes();
	void RegisterTopLevelObjects();
	void RegisterTriggers();
	void RegisterEvents(); // Events should NOT get unregistered

	void UnRegisterCommands();
	void UnRegisterAliases();
	void UnRegisterDataTypes();
	void UnRegisterTopLevelObjects();
	void UnRegisterTriggers();

	void ExecuteFullTemplateEvent();
};

extern LSInterface *pLSInterface;

extern LSMFullTemplate *pModule;
#define printf pLSInterface->Printf

#define EzCheckTriggers(Text) IS_CheckTriggers(pExtension,pLSInterface,hTriggerService,Text)

extern LSTypeDefinition *pStringType;
extern LSTypeDefinition *pIntType;
extern LSTypeDefinition *pUintType;
extern LSTypeDefinition *pBoolType;
extern LSTypeDefinition *pFloatType;
extern LSTypeDefinition *pTimeType;
extern LSTypeDefinition *pByteType;
extern LSTypeDefinition *pIntPtrType;
extern LSTypeDefinition *pBoolPtrType;
extern LSTypeDefinition *pFloatPtrType;
extern LSTypeDefinition *pBytePtrType;

extern char FullTemplate_Version[];

#include "Commands.h"
#include "DataTypes.h"
#include "TopLevelObjects.h"
#include "Events.h"

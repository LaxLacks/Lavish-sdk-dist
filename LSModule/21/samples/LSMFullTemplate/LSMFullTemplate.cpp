//
// LSMFullTemplate
//

// Version guideline: YYYYMMDD
// Add lettering to the end to indicate a new version on the same date, such as 20060305a, 20060305b, etc
// You can also use a standard version numbering system such as 1.00, 1.01, etc. 
// Be aware that for the versioning system, this text is simply compared to another version text from the 
// same extension to check for new versions -- if this version text comes before the compared text (in a 
// dictionary), then an update is available.  Equal text means the version is up to date.  After means this 
// is newer than the compared version.  With that said, use whatever version numbering system you'd like.
#define MODULE_VERSION "20060806"

#include "LSMFullTemplate.h"
#pragma comment(lib,"LSModule.lib")
// The mandatory pre-setup function.  Our name is "LSMFullTemplate", and the class is LSMFullTemplate.
// This sets up a "ModulePath" variable which contains the path to this module in case we want it,
// and a "PluginLog" variable, which contains the path and filename of what we should use for our
// debug logging if we need it.  It also sets up a variable "pExtension" which is the pointer to
// our instanced class.
LSModulePreSetup("LSMFullTemplate",LSMFullTemplate);

// Basic LavishScript datatypes, these get retrieved on startup by our initialize function, so we can
// use them in our Top-Level Objects or custom datatypes
LSTypeDefinition *pStringType=0;
LSTypeDefinition *pIntType=0;
LSTypeDefinition *pUintType=0;
LSTypeDefinition *pBoolType=0;
LSTypeDefinition *pFloatType=0;
LSTypeDefinition *pTimeType=0;
LSTypeDefinition *pByteType=0;
LSTypeDefinition *pIntPtrType=0;
LSTypeDefinition *pBoolPtrType=0;
LSTypeDefinition *pFloatPtrType=0;
LSTypeDefinition *pBytePtrType=0;

LSInterface *pLSInterface=0;

char FullTemplate_Version[]=MODULE_VERSION;


// The constructor of our class.  General initialization cannot be done yet, because we're not given
// the pointer to the LavishScript interface until it is ready for us to initialize.  Just set the
// pointer we have to the interface to 0.  Initialize data members, too.
LSMFullTemplate::LSMFullTemplate(void)
{
}

// Free any remaining resources in the destructor.  This is called when the DLL is unloaded, but
// LavishScript calls the "Shutdown" function first.  Most, if not all, of the shutdown process should
// be done in Shutdown.
LSMFullTemplate::~LSMFullTemplate(void)
{
}

// Initialize is called by Inner Space when the extension should initialize.
bool LSMFullTemplate::Initialize(LSInterface *p_LSInterface)
{
	pLSInterface=p_LSInterface;
	//RegisterModule();

	// retrieve basic ISData types
	pStringType=pLSInterface->FindLSType("string");
	pIntType=pLSInterface->FindLSType("int");
	pUintType=pLSInterface->FindLSType("uint");
	pBoolType=pLSInterface->FindLSType("bool");
	pFloatType=pLSInterface->FindLSType("float");
	pTimeType=pLSInterface->FindLSType("time");
	pByteType=pLSInterface->FindLSType("byte");
	pIntPtrType=pLSInterface->FindLSType("intptr");
	pBoolPtrType=pLSInterface->FindLSType("boolptr");
	pFloatPtrType=pLSInterface->FindLSType("floatptr");
	pBytePtrType=pLSInterface->FindLSType("byteptr");

	RegisterEvents();
	RegisterCommands();
	RegisterAliases();
	RegisterDataTypes();
	RegisterTopLevelObjects();
    RegisterTriggers();

	printf("LSMFullTemplate version %s Loaded",FullTemplate_Version);
	return true;
}

// shutdown sequence
void LSMFullTemplate::Shutdown()
{
	UnRegisterTopLevelObjects();
	UnRegisterDataTypes();
	UnRegisterAliases();
	UnRegisterCommands();
}

void LSMFullTemplate::RegisterCommands()
{
	// add any commands
//	pLSInterface->AddCommand("LSMFullTemplate",CMD_LSMFullTemplate,true,false);
#define COMMAND(name,cmd,parse,hide) pLSInterface->AddCommand(name,cmd,parse,hide);
#include "Commands.h"
#undef COMMAND
}

void LSMFullTemplate::RegisterAliases()
{
	// add any aliases
}

void LSMFullTemplate::RegisterDataTypes()
{
	// add any datatypes
	// pMyType = new MyType;
	// pLSInterface->AddLSType(*pMyType);


#define DATATYPE(_class_,_variable_,_inherits_) {_variable_ = new _class_; pLSInterface->AddLSType(*_variable_); _variable_->SetInheritance(_inherits_);}
#include "DataTypeList.h"
#undef DATATYPE
}

#define EVENT(_eventname_,_variable_) unsigned int _variable_=0
#include "Events.h"
#undef EVENT

void LSMFullTemplate::RegisterEvents()
{
	// Register any events
#define EVENT(_eventname_,_variable_) _variable_=pLSInterface->RegisterEvent(_eventname_)
#include "Events.h"
#undef EVENT
}

void LSMFullTemplate::RegisterTopLevelObjects()
{
	// add any Top-Level Objects
	//pLSInterface->AddTopLevelObject("LSMFullTemplate",TLO_LSMFullTemplate);
#define TOPLEVELOBJECT(name,funcname) pLSInterface->AddTopLevelObject(name,funcname);
#include "TopLevelObjects.h"
#undef TOPLEVELOBJECT
}

void LSMFullTemplate::RegisterTriggers()
{
	// add any Triggers
}

void LSMFullTemplate::UnRegisterCommands()
{
	// remove commands
//	pLSInterface->RemoveCommand("LSMFullTemplate");
#define COMMAND(name,cmd,parse,hide) pLSInterface->RemoveCommand(name);
#include "Commands.h"
#undef COMMAND
}
void LSMFullTemplate::UnRegisterAliases()
{
	// remove aliases
}
void LSMFullTemplate::UnRegisterDataTypes()
{
	// remove data types
#define DATATYPE(_class_,_variable_,_inherits_) {pLSInterface->RemoveLSType(*_variable_); delete _variable_;}
#include "DataTypeList.h"
#undef DATATYPE

}
void LSMFullTemplate::UnRegisterTopLevelObjects()
{
	// remove Top-Level Objects
//	pLSInterface->RemoveTopLevelObject("LSMFullTemplate");
#define TOPLEVELOBJECT(name,funcname) pLSInterface->RemoveTopLevelObject(name);
#include "TopLevelObjects.h"
#undef TOPLEVELOBJECT
}

void LSMFullTemplate::ExecuteFullTemplateEvent()
{
	/* Example of executing a LavishScript event */

	/*
	 * Our event is FullTemplateEvent(int Value).  We will get our int parameter from an external
	 * source, in this case simulated by a local variable called SomeValue.
	 */
	int SomeValue=1234;

	/*
	 * The LSEventHelper class can help us set up our event, to make things easier.  Initialize
	 * the object with the variable we set in Events.h for the event we're executing, which was
	 * Event_FullTemplate
     */
	LSEventHelper Event(Event_FullTemplate);

	/*
	 * The helper class is now set up.  The next part is to add each parameter, in order.  We do
	 * so by using the overloaded AddArg function in the helper class.  The overloads all call
	 * the one for char *, so if we needed to add a type of variable NOT available by overloading,
	 * we could convert to text ourselves first.  In this case, int is set up for us.
	 */
	Event.AddArg(SomeValue); // "int Value" in the event

	/*
	 * Finally, we execute the event! This will execute any event target that is attached to the
	 * event, in arbitrary order. Event targets 
	 */
	Event.Execute();

}


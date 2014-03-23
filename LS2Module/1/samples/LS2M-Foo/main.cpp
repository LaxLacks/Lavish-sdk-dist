// LS2M-Foo - main.cpp
// Entry points for LS2M-Foo

// you should only need to include this to build a module
#include <ls2module.h>
// this is a Visual Studio-specific #pragma directive to force a .lib reference. make sure ls2module.lib is referenced.
#pragma comment(lib,"ls2module.lib")

// we're building a windows DLL in this case..
#include <windows.h>

// the DLL entry points which we're not going to do anything with in this example
BOOL APIENTRY DllMain( HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                      )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) 
	{
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		// sometimes you might need to do cleanup here.
	}
	return TRUE;
}

// these are the functions implemented in LS2Type_Foo.cpp that we will call from this file.
void RegisterFooType();
void UnregisterFooType();

// this is the actual "module" implementation. 
// it is recommended to keep this object slim.
class LS2MFoo : public LavishScript2::ILS2Module
{
public:

	virtual bool Initialize(LavishScript2::LS2BuildInfo &environment_version, LavishScript2::ILS2StandardEnvironment *pEnvironment, LavishScript2::ILS2Array *pInputs, LavishScript2::LS2Exception **ppException)
	{
		// pretty please, call AttachEnvironment with the value passed in here, so that when your custom objects create LS2 objects for you, it doesn't crash instead.
		// you must do this FIRST!
		AttachEnvironment(pEnvironment);


		// do stuff.
		RegisterFooType();
		return true;
	}

	// Shutdown is an Asynchronous operation wherever possible.
	// return false and provide an exception if refusing to shut down. return true ONLY if you will call pShutdownComplete->Signal(). 
	// You do not need to call pShutdownComplete->Signal() within this function; it can come any time during or after, as long as you return true and only call it once.
	virtual bool Shutdown(LavishScript2::ILS2Signal *pShutdownComplete__null_if_too_late_for_asynchronous_shutdown, LavishScript2::LS2Exception **ppException)
	{
		if (pShutdownComplete__null_if_too_late_for_asynchronous_shutdown)
		{
			// in this example, we'll refuse to shut down until it's too late to refuse it.

//			printf("LS2MFoo::Shutdown() aborting!\n");
			*ppException = new LavishScript2::LS2StringException(L"No! Please no! Don't unload me! Think of the kittens.");
			return false;
		}

		// ZOMG I CANNOT ABORT I MUST DO EVERYTHING I CAN BECAUSE UNLOAD IS COMING ANYWAY. YOLOOOOOOOOOOOOOOOOOO!
		// this example isn't complicated enough to really want to do much at this point, 
		//   but this is where you would do your best to try to prevent crashes after your code isn't there.

		// it's possible that a Foo object can exist at the point of calling Shutdown, regardless of whether we unregister...
		UnregisterFooType();

		// in this case, this Module will signal prior to returning true
		pShutdownComplete__null_if_too_late_for_asynchronous_shutdown->Signal();

		// returning true signifies that we have either already signaled, or will be signaling. 
		//    ** if you're not going to signal, you MUST reutrn false and provide an exception
		return true;
	}
	
	static LS2MFoo s_Instance;
};
LS2MFoo LS2MFoo::s_Instance;
	
// this DLL Export must be present for a DLL to provide a Module
extern "C" __declspec(dllexport) LavishScript2::ILS2Module * __stdcall GetLS2Module(unsigned int SDK_VERSION,const char *module_name) 
{
	// if we wanted to, we could use the module_name parameter to differentiate between multiple Modules hosted by the same DLL
	return &LS2MFoo::s_Instance;
}

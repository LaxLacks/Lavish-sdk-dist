#pragma once
#include "DataTypeList.h"

// custom object type declarations 

/* 
 * FullTemplateFoo is a C++ class we will interface with for our "fulltemplatefoo" object type
 */
class FullTemplateFoo
{
public:
	FullTemplateFoo()
	{
		Apples=0;
		Llamas=54321;
		Clouds[0]=2.5f;
		Clouds[1]=5.0f;
		Clouds[2]=7.5f;
		Clouds[3]=10.0f;
		Clouds[4]=12.5f;
		Clouds[5]=15.0f;
		Clouds[6]=17.5f;
		Clouds[7]=20.0f;
		Clouds[8]=22.5f;
		SetBar("fulltemplatefoo Bar");
	}
	~FullTemplateFoo()
	{
	}

	void SetBar(const char *NewBar)
	{
		strncpy(Bar,NewBar,sizeof(Bar));
		Bar[sizeof(Bar)-1]=0;
	}

	void ApplesInc()
	{
		Apples++;
	}

	void SetLlamas(unsigned int NewLlamas)
	{
		Llamas=NewLlamas;
	}

	unsigned int Llamas;

	unsigned int Apples;

	float Clouds[9];

	char Bar[64];
};

class FullTemplateFooType : public LSTypeDefinition
{
public:
	// All data members (retrieving data) should be listed in this enumeration
	enum FullTemplateFooTypeMembers
	{
		Apples,
		Llamas,
		Bar,
		Cloud,
	};
	// All data methods (performing actions on or with the object) should be listed in this enumeration
	enum FullTemplateFooTypeMethods
	{
		ApplesInc,
		SetLlamas,
		SetBar,
		xApples, // Example for name conflict resolution
	};

	FullTemplateFooType() : LSTypeDefinition("fulltemplatefoo")
	{
		/*
		 * Use the TypeMember macro to activate each member, or use AddMember if the name
		 * would conflict with something else in the source code.
		 */

		TypeMember(Apples);
		TypeMember(Llamas);
		TypeMember(Bar);
		TypeMember(Cloud);
		// AddMember(Apples,"Apples")

		/*
		 * Use the TypeMethod macro to activate each method, or use AddMethod if the name
		 * would conflict with something else in the source code.
		 */
		TypeMethod(ApplesInc);
		TypeMethod(SetLlamas);
		TypeMethod(SetBar);

		AddMethod(xApples,"Apples");


	}



	/*********************************************************************************************
	 * Basic operations 
	 *
	 */
	
	/* Retrieve some object, a member of an object of this type, using the given arguments */
	virtual bool GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER pMember, int argc, char *argv[], LSOBJECT &Object);

	/* Perform some action on behalf of this object */
	virtual bool GetMethod(LSOBJECTDATA &ObjectData, PLSTYPEMETHOD pMethod, int argc, char *argv[]);

	/* Retrieve some object, an index of an object of this type, using the given arguments */
	virtual int GetIndex(LSOBJECTDATA ObjectData, int argc, char *argv[], LSOBJECT &Object);

	/* Reduce an object of this type to text */
	virtual bool ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen);



	/*********************************************************************************************
	 * Inheritance
	 *
	 * Inheritance is a fairly standard OOP concept.  LavishScript allows single-inheritance most
	 * easily, but can be coaxed into using any inheritance model.  The macros below are defined
	 * in LSType.h.
	 */

	/* The INHERITDIRECT macro can be used if any given object of this type "is a" object of the
	   inherited type. */
#if 0
	 INHERITDIRECT(pStringType);
#endif

	/* The INHERITINDIRECT macro can be used if conversion is required to or from a given
	   object type.  This is slightly more complex than INHERITDIRECT, because the complete routine
	   for conversion from LSOBJECTDATA ObjectData of this type to LSOBJECTDATA Temp of the 
	   inherited type must be performed as the second argument.  The third argument can then apply any
	   change to Temp back to ObjectData. */
#if 0
      INHERITINDIRECT(pStringType,Temp.CharPtr=pString->c_str(),pString->Set(Temp.CharPtr))
#endif



	/*********************************************************************************************
	 * Variable initialization and destruction (allows LavishScript variables to be created with this type)
	 *
	 * If variables are not to be supported by the object type, simply return false from the InitVariable
	 * function, or remove the functions below (they will be replaced by the defaults, which do not allow
	 * variables)
	 *
	 */

	/* Initialize an object of this type, potentially with an object sub-type */
    virtual bool InitVariable(LSOBJECTDATA &ObjectData, const char *SubType);

	/* As the second stage of initializing the object, assign the given value(s).  This will
	 * ALWAYS be called immediately after a successful InitVariable, even if no value is given. */
	virtual bool FromText(LSOBJECTDATA &ObjectData, int argc, char *argv[]);

	/* Destroy an object of this type */
	virtual void FreeVariable(LSOBJECTDATA &ObjectData);



	/*********************************************************************************************
	 * Variable type redirection
	 *
	 * In many cases, there is reason to keep a separate datatype for variables, since variables
	 * may be modified.  For example, the string type works on const char *, which would be unsafe
	 * for a LavishScript variable to modify -- the text may be a constant built right into a module.  
	 * The string type redirects variables to the mutablestring type, which works on utf8string *
	 * and inherits the string type (by accessing the const char *).
     */

	/* This function indicates that the variable type is >this< type.  GetVariableType() can also
	   return 0 (NULL) to indicate that variables are not allowed. */
	virtual LSTypeDefinition *GetVariableType()
	{
		return this; /* Redirection example: return pMutableStringType; */
	}


};

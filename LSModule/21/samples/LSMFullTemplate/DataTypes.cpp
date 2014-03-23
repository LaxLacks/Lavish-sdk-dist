#include "LSMFullTemplate.h"
#define DATATYPE(_class_,_variable_,_inherits_) class _class_ *_variable_=0
#include "DataTypeList.h"
#undef DATATYPE

/* Set up an easy way to access the object, given its 32-bit representation (e.g. a pointer to
   a class or structure) */
#define pFoo ((FullTemplateFoo*)ObjectData.Ptr)


bool FullTemplateFooType::GetMember(LSOBJECTDATA ObjectData, PLSTYPEMEMBER pMember, int argc, char *argv[], LSOBJECT &Object)
{
/*******************************************
 * Parameters
 *
 * [in] LSOBJECTDATA ObjectData: ObjectData is a 32-bit value that can be accessed in any number of different ways
 *        by way of union.  Most commonly, ObjectData.Ptr, ObjectData.DWord, or ObjectData.CharPtr are useful.  This
 *        value is the representation of some object of this object type.  "fulltemplatefoo" works on FullTemplateFoo*
 *        so ObjectData is a FullTemplateFoo*
 *
 * [in] PLSTYPEMEMBER pMember: pMember is a pointer to the information on the member to be retrieved, including its
 *        Name and ID.  We use the ID in a switch statement in order to quickly process the member, since the Name
 *        has already been resolved by the LavishScript engine.
 *
 * [in] int argc, char *argv[]: argc and argv are *nearly* standard C console program parameters.  The difference here
 *        is that the name of the member is NOT given as the first argument (in contrast to LavishScript commands).
 *        Therefore, argc is 0 unless arguments are specifically given to the member retrieval.
 *
 * [out] LSOBJECT &Object: Object is an object that is the result of the member retrieval operation.  An LSOBJECT
 *        is an LSOBJECTDATA, in addition to a separate Type value used to give the value context (in the first
 *        parameter, the context is already known to be the fulltemplatefoo object type, so it is LSOBJECTDATA).
 *
 */

/*******************************************
 * Return Value
 *
 * The return value for this function is very simple.  If the member retrieval fails for any reason, return false.
 * If Object is a valid object and the member retrieval is successful, return true.
 *
 */

	/* Validate the pointer */
	if (!pFoo)
		return false;

	/* Perform the given member retrieval */
	switch(pMember->ID)
	{
	case Apples:
		Object.DWord=pFoo->Apples;
		Object.Type=pUintType;
		return true;
	case Llamas:
		Object.DWord=pFoo->Llamas;
		Object.Type=pUintType;
		return true;
	case Bar:
		Object.CharPtr=pFoo->Bar;
		Object.Type=pStringType;
		return true;
	case Cloud:
		if (argc)
		{
			unsigned int N=atoi(argv[0]);
			// Any arrays in LavishScript should be 1-based for consistency. Convert to 0-based by subtracting 1
			N--;
			if (N>=9) // Out of range. Valid values are 0,1,2,3,4,5,6,7,8 at this point
				return false;
			Object.Float=pFoo->Clouds[N];
			Object.Type=pFloatType;
			return true;
		}
		return false;
	}
	return false;
}

bool FullTemplateFooType::GetMethod(LSOBJECTDATA &ObjectData, PLSTYPEMETHOD pMethod, int argc, char *argv[])
{
/*******************************************
 * Parameters
 *
 * [in] LSOBJECTDATA ObjectData: ObjectData is a 32-bit value that can be accessed in any number of different ways
 *        by way of union.  Most commonly, ObjectData.Ptr, ObjectData.DWord, or ObjectData.CharPtr are useful.  This
 *        value is the representation of some object of this object type.  "fulltemplatefoo" works on FullTemplateFoo*
 *        so ObjectData is a FullTemplateFoo*
 *
 * [in] PLSTYPEMETHOD pMethod: pMethod is a pointer to the information on the method to be retrieved, including its
 *        Name and ID.  We use the ID in a switch statement in order to quickly process the method, since the Name
 *        has already been resolved by the LavishScript engine.
 *
 * [in] int argc, char *argv[]: argc and argv are *nearly* standard C console program parameters.  The difference here
 *        is that the name of the method is NOT given as the first argument (in contrast to LavishScript commands).
 *        Therefore, argc is 0 unless arguments are specifically given to the method retrieval.
 */

/*******************************************
 * Return Value
 *
 * The return value for this function is very simple.  If the method execution fails for any reason, OR the object
 * is destroyed during execution, return false.  Otherwise, return true (indicating the object still exists AND
 * the method execution succeeded).
 *
 */
	/* Validate the pointer */
	if (!pFoo)
		return false;

	/* Perform the given member retrieval */
	switch(pMethod->ID)
	{
	case ApplesInc:
		pFoo->ApplesInc();
		return true;
	case SetLlamas:
		if (argc)
		{
			pFoo->SetLlamas(atoi(argv[0]));
			return true;
		}
		return false;
	case SetBar:
		if (argc)
		{
			pFoo->SetBar(argv[0]);
			return true;
		}
		return false;
	case xApples:
		return false;
	}
	return false;
}

/* Retrieve some object, an index of an object of this type, using the given arguments */
int FullTemplateFooType::GetIndex(LSOBJECTDATA ObjectData, int argc, char *argv[], LSOBJECT &Object)
{
	// Quick and dirty way to reroute index retrieval to the Cloud member
	return GetMemberEx(ObjectData,"Cloud",argc,argv,Object);
}

/* Reduce an object of this type to text */
bool FullTemplateFooType::ToText(LSOBJECTDATA ObjectData, char *buf, unsigned int buflen)
{
	strncpy(buf,pFoo->Bar,buflen);
	buf[buflen-1]=0;
	return true;
}

/* 
 * Initialize a new variable of this type, potentially with an object sub-type.  In this case,
 * since our object type works on FullTemplateFoo*, we instantiate a FullTemplateFoo
 * structure.  If our object type should not support variables, we could simply return false.
*/
bool FullTemplateFooType::InitVariable(LSOBJECTDATA &ObjectData, const char *SubType)
{
	if (SubType && SubType[0])
	{
		// Our type does not support sub-types
		return false;
	}
	// Allocate our FullTemplateFoo object
	ObjectData.Ptr=new FullTemplateFoo;
	return true;
}

/* As the second stage of initializing a variable of this type, assign the given value(s).  This will
 * ALWAYS be called immediately after a successful InitVariable, even if no value is given. */
bool FullTemplateFooType::FromText(LSOBJECTDATA &ObjectData, int argc, char *argv[])
{
	if (argc)
	{
		// our sample sets Bar with the (first) initialization parameter
		pFoo->SetBar(argv[0]);
	}
	return true;
}

/* Destroy a variable of this type */
void FullTemplateFooType::FreeVariable(LSOBJECTDATA &ObjectData)
{
	// Destroy our FullTemplateFoo object
	delete pFoo;
}

#undef pFoo

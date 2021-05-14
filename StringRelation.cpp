#include "StringRelation.h"
#include <unistd.h>

/*
 * Assign SetMembers.
 */
void StringRelation::setInput(SetOfStrings *s)
{
	SetMembers=s;
}

/*
 * It splits a pair "a,b" into a vector of strings "a", "b".
 * Returns the string vector.
 */
vector<string> StringRelation::makeTokens(string pair){
	vector<string> out;
	vector<string>::iterator p;
	char *a = new char [pair.length()+1];
	strcpy(a,pair.c_str());
	char* token = strtok(a,",");
			int index=0;
				while (token != NULL) {
					out.push_back(token);
					token = strtok(NULL, ",");
					index++;
				}
	return out;
}

/*
 * Returns the equivalence class as a set object.
 */

/* an equivalence relation would be if all elements are reflexive, transitive and symmetric
 * but if one of the elements is missing, and it's still a reflexive and transitive relation, we only display the
 * class for which all three exist (and a path exists from 1->2, 2->3 and 1->3 */

SetOfStrings *StringRelation::computeEquivalenceClass(string element){
	SetOfStrings *out = new SetOfStrings();
	int i, j;
	vector<string> createdTokens;
	if(isEquivalence() == true)
	{
		for(i = 0; i < size(); i++)
		{
			createdTokens = makeTokens(returnElement(i));
			for(j = 0; j < SetMembers->size(); j++)
			{
				if((createdTokens[0] == element) && (createdTokens[1] == SetMembers->returnElement(j)))
					out->insertElement(createdTokens[1]);
			}
		}
	}
	return out;
}

/*
 * Returns true if the relation is reflexive.
 * Otherwise, returns false.
 */
bool StringRelation::isReflexive(){
	/* The purpose of this function is to check if every element of the set has a pair where it is
	 * related to itself, which would make the entire relation reflexive. */
	int i, checkReflex = 0;

	// A string that stores the relation returned from the returnElement(index) function
	string obtainedString;

	// A vector of strings that stores the result of the 'makeTokens' function
	vector<string> createdTokens;
	for(i = 0; i < size(); i++)
	{
		obtainedString = returnElement(i);
		createdTokens = makeTokens(obtainedString);
		if(createdTokens[0] == createdTokens[1])
			checkReflex++;
	}

	/* If the number of reflexive relations is the same as the number of set elements in the text file,
	 * the function has to be reflexive. */
	if(checkReflex == SetMembers->size())
		return true;
	return false;
}

/*
 * Returns true if the relation is symmetric.
 * Otherwise, returns false.
 */
bool StringRelation::isSymmetric(){
	/* The purpose of this function is to check if every pair in the set is SYMMETRIC, that is if a pair (a,b)
	 * exists, there SHOULD be a pair (b,a) in the same file to satisfy symmetry conditions for the entire relation. */

	// Integer initialisation for the 'for' loops and symmetry checks
	int i, j, prelimCheck = 0, checkSymm = 0;

	string obtainedString, temporaryVar;
	vector<string> createdTokens, tempTokens;

	// Every element (including the ones that are reflexive) must be symmetric
	for(i = 0; i < size(); i++)
	{
		temporaryVar = returnElement(i);
		tempTokens = makeTokens(temporaryVar);
		prelimCheck++;
		for(j = 0; j < size(); j++)
		{
			obtainedString = returnElement(j);
			createdTokens = makeTokens(obtainedString);
			if(createdTokens[0] == tempTokens[1] && tempTokens[0] == createdTokens[1])
				checkSymm++;
		}
	}
	// Every element has a symmetric match, then the entire relation set is symmetric.
	if(prelimCheck == checkSymm)
		return true;
	return false;
}

/*
 * Returns true if the relation is transitive.
 * Otherwise, returns false.
 */


bool StringRelation::isTransitive(){
	/* The purpose of this function is to test if in a relation, every pair has a path such that if (a,b) and (b,c)
	 * exist, there HAS to be a pair (a,c) for the relation to be TRANSITIVE. */
	int i, j, k, prelimCheck = 0, checkTrans = 0;
	vector<string> createdTokens, tempTokens;

	for(i = 0; i < size(); i++)
	{
		/* The pair returned must be stored in a variable of type 'string' and converted into tokens of
		type 'string vector'. This stores a pair of type (a,b). */
		tempTokens = makeTokens(elements[i]);
		if(tempTokens[0] != tempTokens[1])
		{
			createdTokens = tempTokens;
			for(j = 0; j < size(); j++)
			{
				/* Obtained string contains the second pair and then converts into tokens. */
				tempTokens = makeTokens(elements[j]);

				/* If the second pair is of the type (b,c) where the first pair's second element and the second pair's first
						element are both 'b', this condition would be satisfied. */
				if(tempTokens[0] == createdTokens[1] && tempTokens[0] != tempTokens[1])
				{
					// The pair is inserted into tempTokens that stores the matches so far.
					createdTokens.push_back(tempTokens[0]);
					createdTokens.push_back(tempTokens[1]);

					// (a,b) and (b,c) exist
					prelimCheck++;

					for(k = 0; k < size(); k++)
					{
						tempTokens = makeTokens(elements[k]);

						// If (a,c) is found, the pairs have a transitive path connecting them together.
						if(tempTokens[0] == createdTokens[0] && tempTokens[1] == createdTokens[3])
							checkTrans++;
					}
					// After checking for a particular (b,c) match,  the inserted tokens must be removed
					tempTokens.pop_back();
					tempTokens.pop_back();
				}
				else
					tempTokens = makeTokens(elements[i]);
			}
		}
	}
	if(checkTrans == prelimCheck)
		return true;
	return false;
}

/*
 * Returns true if the relation is the equivalence relation.
 * Otherwise, returns false.
 */
bool StringRelation::isEquivalence(){
	bool checkReflex = isReflexive();
	bool checkSymm = isSymmetric();
	bool checkTrans = isTransitive();
	if((checkReflex == true && checkSymm == true) && checkTrans == true)
		return true;
	return false;
}

#include "Hashlife.h"


struct macrocell
{
	Vec_2 dimensions;
	macrocell* LL;
	macrocell* LR;
	macrocell* UL;
	macrocell* UR;
	macrocell* Result;
}
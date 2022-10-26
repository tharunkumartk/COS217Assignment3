#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define ASSURE(i) assure(i, __LINE__)

/*--------------------------------------------------------------------*/

/* If !iSuccessful, print a message to stdout indicating that the
   test at line iLineNum failed. */

static void assure(int iSuccessful, int iLineNum)
{
   if (! iSuccessful)
   {
      printf("Test at line %d failed.\n", iLineNum);
      fflush(stdout);
   }
}
int main(int argc, char *argv[]) {
    SymTable_T oSymTable;
   char acJeter[] = "Jeter";
   char acMantle[] = "Mantle";
   char acGehrig[] = "Gehrig";
   char acRuth[] = "Ruth";
   char acShortstop[] = "Shortstop";
   char acCenterField[] = "Center Field";
   char acFirstBase[] = "First Base";
   char acRightField[] = "Right Field";

   char acBrown[] = "Brown";
   
   char *pcValue;
   int iSuccessful;
   int iFound;
   size_t uLength;

   printf("------------------------------------------------------\n");
   printf("Testing the most basic SymTable functions.\n");
       printf("please");
   printf("No output should appear here:\n");

   /*  */


   /* Test SymTable_new(). */

   oSymTable = SymTable_new();
   ASSURE(oSymTable != NULL);
   /* Test SymTable_put() and SymTable_getLength(). */

   iSuccessful = SymTable_put(oSymTable, acJeter, acShortstop);
   ASSURE(iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 1);

   iSuccessful = SymTable_put(oSymTable, acMantle, acCenterField);
   ASSURE(iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 2);

   iSuccessful = SymTable_put(oSymTable, acGehrig, acFirstBase);
   ASSURE(iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 3);

   iSuccessful = SymTable_put(oSymTable, acRuth, acRightField);
   ASSURE(iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 4);

   /* Try to insert duplicate to first key entered */
   iSuccessful = SymTable_put(oSymTable, acJeter, acCenterField);
   ASSURE(! iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 4);

   /* Try to insert duplicate to last key entered */
   iSuccessful = SymTable_put(oSymTable, acRuth, acCenterField);
   ASSURE(! iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 4);

   
   /* Test SymTable_contains(). */

   iFound = SymTable_contains(oSymTable, acJeter);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acMantle);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acGehrig);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, acRuth);
   ASSURE(iFound);

   iFound = SymTable_contains(oSymTable, "Clemens");
   ASSURE(! iFound);

   iFound = SymTable_contains(oSymTable, "Maris");
   ASSURE(! iFound);

   /* Test SymTable_get(). */

   pcValue = (char*)SymTable_get(oSymTable, acJeter);
   ASSURE(pcValue == acShortstop);

   pcValue = (char*)SymTable_get(oSymTable, acMantle);
   ASSURE(pcValue == acCenterField);

   pcValue = (char*)SymTable_get(oSymTable, acGehrig);
   ASSURE(pcValue == acFirstBase);

   pcValue = (char*)SymTable_get(oSymTable, acRuth);
   ASSURE(pcValue == acRightField);

   pcValue = (char*)SymTable_get(oSymTable, "Clemens");
   ASSURE(pcValue == NULL);

   pcValue = (char*)SymTable_get(oSymTable, "Maris");
   ASSURE(pcValue == NULL);
   
   /* Test SymTable_replace(). */

   pcValue = (char*)
      SymTable_replace(oSymTable, acMantle, acFirstBase);
   ASSURE(pcValue == acCenterField);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 4);

   pcValue = (char*)SymTable_get(oSymTable, acMantle);
   ASSURE(pcValue == acFirstBase);

   pcValue = (char*)
      SymTable_replace(oSymTable, "Clemens", acRightField);
   ASSURE(pcValue == NULL);

   pcValue = (char*)SymTable_replace(oSymTable, "Maris", acRightField);
   ASSURE(pcValue == NULL);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 4);


   /* Insert key with NULL value */
   iSuccessful = SymTable_put(oSymTable, acBrown, NULL);
   ASSURE(iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 5);

   /* Try to insert duplicate of key that had NULL value */
   iSuccessful = SymTable_put(oSymTable, acBrown, acShortstop);
   ASSURE(! iSuccessful);

   uLength = SymTable_getLength(oSymTable);
   ASSURE(uLength == 5);
 

   /* Test SymTable_free(). */

   SymTable_free(oSymTable);
}




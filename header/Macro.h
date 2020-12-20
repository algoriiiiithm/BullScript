#ifndef MACRO_
#define MACRO_

/*some general macro definations*/
#define IsEmpty(S) ((S)->elementsNumber == 0)
#define Push(S, X) List_InsertIntoFirst((S), (X))
#define Pop(S, D) List_RemoveFirstElement((S), (D))

#endif
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct NL_OBJ*
addNode(psizeRMXbuffer, pRMXBuffer, basedOnNode, basedOnRoom, inRoom)
unsigned int basedOnNode, basedOnRoom, inRoom;
struct RMX(**pRMXBuffer);
size_t* psizeRMXbuffer;
{
	struct RMX(*RMXBuffer) = *pRMXBuffer;
	struct Room(*currentRoom);
#define rmxpointer __based(RMXBuffer)
#define roompointer __based(currentRoom)
#include "GameFormatRMX.h"

	struct NL_OBJ * objtoinherit = NULL;
	size_t nextroomindex = RMXBuffer->AmountRooms, currromindx = -1;
	//struct Room(*roomtoextend) = NULL;

	for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms; ++RoomIndex) {
		if (currentRoom->RoomHash == inRoom) {
			//roomtoextend = currentRoom;
			// get next room beginning address - to extend last with new objects
			currromindx = RoomIndex;
			break;
		}
	}
	assert(currromindx != -1);//&& roomtoextend);

	struct NL_OBJ * realpos;
	//currentRoom = RMXBuffer->Rooms[nextroomindex - 1];
	*psizeRMXbuffer += sizeof(struct NL_OBJ) + 0x60;
	RMXBuffer = realloc(RMXBuffer, *psizeRMXbuffer);
	*pRMXBuffer = RMXBuffer;
	currentRoom = RMXBuffer->Rooms[nextroomindex];
	/*for (struct NL_OBJ* p = currentRoom->nodes; p < 1[&currentRoom->nodes]; ++p) {
		if (p >= currentRoom + 1) {
			for (struct NL_OBJ* (currentObject) = *(struct NL_OBJ roompointer * __ptr32*)p; currentObject != currentRoom;) {
				struct NL_OBJ* (lastObject) = currentObject;
				currentObject = currentObject->Next;
				if (lastObject->Next != currentRoom)
					lastObject->Next += 1;
				if (lastObject->Previous != currentRoom)
					lastObject->Previous += 1;
			}
		}
			*p += sizeof(struct NL_OBJ);
		} }
	}*/
	memmove((unsigned char*)((struct NL_OBJ *)currentRoom + 1) + 0x60,
		(unsigned char*)currentRoom,
		(*psizeRMXbuffer - (((unsigned char*)((struct NL_OBJ*)currentRoom + 1) + 0x60) - (unsigned char*)RMXBuffer)));
	for (size_t RoomIndex = nextroomindex; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms + 1; ++RoomIndex)
		RMXBuffer->Rooms[RoomIndex] = (unsigned char rmxpointer *)RMXBuffer->Rooms[RoomIndex] + sizeof(struct NL_OBJ) + 0x60;
	objtoinherit = NULL;
	for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms && !objtoinherit; ++RoomIndex) {
		if (currentRoom->RoomHash == basedOnRoom)
			for (struct NL_OBJ* (currentObject) = currentRoom->FirstObject; currentObject != currentRoom; currentObject = currentObject->Next)
				if (currentObject->ObjectHash == basedOnNode) {
					objtoinherit = currentObject;
					break;
				}
	}
	currentRoom = RMXBuffer->Rooms[nextroomindex];
	memset((unsigned char*)currentRoom + (sizeof(struct NL_OBJ)), 0, 0x60);
	realpos = (struct NL_OBJ *)currentRoom - 1;
	currentRoom = RMXBuffer->Rooms[currromindx];
	assert(objtoinherit);
	memcpy(realpos, objtoinherit, sizeof(struct NL_OBJ));
	struct NL_OBJ *first_obj = currentRoom->FirstObject;
	realpos->Previous = NULL;
	currentRoom->FirstObject->Previous = realpos;
	currentRoom->FirstObject = realpos;
	assert(nextroomindex > 1);
	realpos->Next = first_obj;
	return realpos;
}

struct NL_OBJ*
find_node (RMXBuffer, node, room)
unsigned int node, room;
struct RMX(*RMXBuffer);
{
	struct Room(*currentRoom);
#define rmxpointer __based(RMXBuffer)
#define roompointer __based(currentRoom)
#include "GameFormatRMX.h"

	for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms; ++RoomIndex)
		if (currentRoom->RoomHash == room)
			for (struct NL_OBJ* (currentObject) = currentRoom->FirstObject; currentObject != currentRoom; currentObject = currentObject->Next)
				if (currentObject->ObjectHash == node)
					return currentObject;
	return NULL;
}
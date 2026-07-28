#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
//#include "cmn.h"
struct NL_OBJ*
addNode(size_t* psizeRMXbuffer, struct RMX(**pRMXBuffer), unsigned int basedOnNode, unsigned int basedOnRoom, unsigned int inRoom)
{
	struct RMX(*RMXBuffer_) = *pRMXBuffer;
	struct Room(*currentRoom_);
	{
#define rmxpointer __based(RMXBuffer_)
#define roompointer __based(currentRoom_)
#include "GameFormatRMX.h"

		struct RMX(*&RMXBuffer) = (struct RMX*&)RMXBuffer_;
		struct Room(*&currentRoom) = (struct Room*&)currentRoom_;

		struct NL_OBJ* objtoinherit = NULL;
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

		struct NL_OBJ* realpos;
		//currentRoom = RMXBuffer->Rooms[nextroomindex - 1];
		*psizeRMXbuffer += sizeof(struct NL_OBJ) + 0x60;
		RMXBuffer = (decltype(+RMXBuffer))realloc(RMXBuffer, *psizeRMXbuffer);
		*pRMXBuffer = (decltype(*pRMXBuffer))RMXBuffer;
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
		memmove((unsigned char*)((struct NL_OBJ*)currentRoom + 1) + 0x60,
			(unsigned char*)currentRoom,
			(*psizeRMXbuffer - (((unsigned char*)((struct NL_OBJ*)currentRoom + 1) + 0x60) - (unsigned char*)RMXBuffer)));
		for (size_t RoomIndex = nextroomindex;
			currentRoom = RMXBuffer->Rooms[RoomIndex],
			RoomIndex < RMXBuffer->AmountRooms + 1; ++RoomIndex)

			RMXBuffer->Rooms[RoomIndex] += sizeof(struct NL_OBJ) + 0x60;
		objtoinherit = NULL;
		for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms && !objtoinherit; ++RoomIndex) {
			if (currentRoom->RoomHash == basedOnRoom)
				for (struct NL_OBJ* (currentObject) = (NL_OBJ*)currentRoom->FirstObject; currentObject != (NL_OBJ*)currentRoom; currentObject = currentObject->Next)
					if (currentObject->ObjectHash == basedOnNode) {
						objtoinherit = currentObject;
						break;
					}
		}
		currentRoom = RMXBuffer->Rooms[nextroomindex];
		memset((unsigned char*)currentRoom + (sizeof(struct NL_OBJ)), 0, 0x60);
		realpos = (NL_OBJ*)currentRoom - 1;
		currentRoom = RMXBuffer->Rooms[currromindx];
		assert(objtoinherit);
		memcpy(realpos, objtoinherit, sizeof(struct NL_OBJ));
		struct NL_OBJ* first_obj = (NL_OBJ*)currentRoom->FirstObject;
		realpos->Previous = NULL;
		currentRoom->FirstObject->Previous = (decltype(NL_OBJ::Previous))realpos;
		currentRoom->FirstObject = (decltype(Room::FirstObject))realpos;
		assert(nextroomindex > 1);
		realpos->Next = (decltype(NL_OBJ::Next))first_obj;
		return (::NL_OBJ*)realpos;
	}
}

struct NL_OBJ*
find_node (struct RMX(*RMXBuffer_), unsigned int node, unsigned int room)
{
	struct Room(*currentRoom_);
#define rmxpointer __based(RMXBuffer_)
#define roompointer __based(currentRoom_)
#include "GameFormatRMX.h"
	{
		struct RMX(*&RMXBuffer) = (struct RMX*&)RMXBuffer_;
		struct Room(*&currentRoom) = (struct Room*&)currentRoom_;

		for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms; ++RoomIndex)
			if (currentRoom->RoomHash == room)
				for (struct NL_OBJ* (currentObject) = currentRoom->FirstObject; currentObject != (void*)currentRoom; currentObject = currentObject->Next)
					if (currentObject->ObjectHash == node)
						return (::NL_OBJ*)currentObject;
		return NULL;
	}
}
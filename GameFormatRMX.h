typedef unsigned long dword; //32bit word
typedef unsigned short word; //16bit word
struct FloatingPosition
{
    float x,y,z,d;
    //d Indicates the position is point or vertex - the default value is point (0x3F80=1)
};
struct NL_OBJ {
    struct NL_OBJ roompointer *__ptr32(Previous), roompointer *__ptr32(Next);
    char unknown18[0x18];
    struct FloatingPosition ObjectTransform, //XZY object transformation
            ObjectRotation; //XZY object rotation in degrees (clockwise)
    char unknown10[0x10];
    struct FloatingPosition TriggerPositionObviously;
    dword unknown4,
    ObjectHash,  //Object identify hash (used to call this object)
    ObjectType;  //What type is the current object (follows the numerical way of pointer to such are structured in the room header)
    char unknown10_1[0x10];
    dword ScriptHash; //Hash from SCX file to drive this object
    char unknownE8[0xE8];
    dword FlagOrPickable, //EN_PICKABLE = 0x200
    unknown4_1;
    word MeshObjIndex,  //Indicates mesh-obj index from the current zone-file which is loaded (every NL_OBJ points to the current loaded zone objects NOT to the RoomObj::RoomHash origin map!)
    BehaviorIndex,  //Indicates Behavior struct index from the script-file
    AmountFuncsInvoked;  //Indicates Amount of the functions invoked at the end of the object
    char unknown26[0x26]; //??
    dword TimesMemAllocated; //how many blocks of 0x40 bytes will be allocated to be used by function 'colInsertObjectIntoList'
//If copying objects over watch out for this value otherwise game engine could overflow and thus cause bugs
    char Bytes70[0x70];  //Some positions linked with object lightning??
    struct {
        dword dwEventIdToInvoke; //When this id is received into the callback function - this Func is invoked
        dword dwFuncNum; //Number of function invoked from EVX file
    } FancyFuncInvocation[]; //as AmountFuncsInvoked
};
struct NODE { struct NL_OBJ roompointer* __ptr32(FirstObject), roompointer* __ptr32(LastOne); };
struct Room
{
    char unknown70[0x70];
    struct FloatingPosition RoomTransform,  //The room XZY transform
        //Down are positions of two points which are the room visibility limits
        RoomLimitPoint0,  //The position of the first point
        RoomLimitPoint1;  //The position of the second point
    char unknown8[8];
    dword RoomHash,  //This is the hashed room name mapped in this structure (the hash is searched by the TR 6 engine in the current loaded zone for room resources)
        unknownC[3];
    union {
        struct NL_OBJ roompointer* __ptr32(FirstObject), roompointer* __ptr32(LastOne);
        struct {
            struct NODE nodes[13];
            struct NODE potnodes[5];
            unsigned char fs[0x10];
            struct NODE potnodes1[3];
        };
    };
};
struct RMX
{
    float Version;             // 3.6f
    char unknownC[0xC];
    dword AmountRooms;
    struct Room rmxpointer*__ptr32(Rooms)[];          // Pointer to the Mapped room  (Map struct) as amount of AmountMaps
};

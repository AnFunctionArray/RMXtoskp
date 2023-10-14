typedef unsigned long dword; //32bit word
typedef unsigned short word; //16bit word
struct Mesh {
	void(meshpointer* __ptr32(next));  //Structure size
	signed int nuvscords, unknown4_0,
		AmountVertices, // Amount  of vertices
		unknown4,
		AmountIndexes,   // Amount  of indexes
		unknown4_1,
		AmountGroups,  // Amount  of groups
		unknown14[5];
	struct Vertex {
		float X, Y, Z, U, V, U1, V1;
		dword Unknown, SpecularColor, DiffuseColor;
	} vertexData[];  //vertex data of number as  AmountVertices
#define indexData(currentMesh)((word*)((currentMesh).vertexData+(currentMesh).AmountVertices))
#define groupData(currentMesh)((struct Group*)(indexData(currentMesh)+(currentMesh).AmountIndexes))
};
struct Group {
	signed long NumFaces,
		NumVertices,
		StartIndex,
		MaterIndex;
	short uvxcoords[4];
	unsigned long lastindex;
	//short uvxindices[3];
	unsigned long PrimitiveType;
	float PosX1,
		PosZ1,
		PosY1,
		IsPoint1,
		PosX2,
		PosZ2,
		PosY2,
		IsPoint2;
};
struct Meshes {
	dword AmountRooms;
	struct RoomMesh {
		dword RoomHash; //Room Hash used and translated from RMX::RoomObj::roomHash
		struct Mesh Room;
	} MeshRooms;
};
void(*MeshesObjects(a));
typedef struct MeshesObjects {
	dword AmountObjects;
	struct Mesh Objects;
} typeMeshesObjects;
#define MeshesObjects ((struct MeshesObjects *(*)())MeshesObjects)
struct Zone {
	dword Size; //32
	struct textures(zonepointer* __ptr32(TextPtr)); //Texture struct pointer
	struct Objs zonepointer* __ptr32(ObjsPtr); //Objs struct pointer
	struct Meshes zonepointer* __ptr32(MshPtr); //Meshes struct pointer
	void* __ptr32(StatColPtr); //StaticCol struct ponter
};
struct ObjInfo {
	float Pos1Ang[3];  //Those two vector creating
	dword unknown4;
	float Pos2Ang[3];  //an cube which describes the object parameters in 3d space (used only in pushable objects to secure the collision)
	dword unknown4_1,
		ObjectCodeBin[];   /*This is a binary code which describes the used objects in this info
		here is what the binary code means:
				0xFFFFFFFE - begin scope
		0xFFFFFFFC - begin float values 0x18 (first 3 floats creates a position vector and the second-rotation vector-translating the objects in the current scope)
		0xFFFFFFFD - end scope
		0xFFFFFFFF - end object_node
		in every scope you can include object meshes by index like this:
		DWORD MeshObj[4] like the first dword value is the index of the mesh-object*/
};
struct textures {
	dword	AmountTextInfo, unknown4, AmountTextHead, unknown4_1;

	struct TextInfo { dword unknown4, unknown4_1, primarytexture, unknown4_2, secondarytexture, unknown4_3; } textureinfo[];

#define textureheaders(currentTextures)(struct TextHead*)((currentTextures).textureinfo+(currentTextures).AmountTextInfo)
#define texturedata(currentTextures)(char *)(textureheaders(currentTextures)+(currentTextures).AmountTextHead)
};

struct TextHead {
	char format[4];
	dword unknownc[3], levels, xsize, ysize, datasize, unknown8[2];
};
struct ObjHead {
	float Positions[7]; //Some object Transforms
	void* PtrShad; //Points to object Shader (struct Shader - file format)
	dword ObjHash, //Hashed object name
		unknown2c[11]; //in every object is same
};
struct Objs {
	dword AmountObjHead, AmountObjInfoPtrs;
	struct ObjInfo zonepointer* __ptr32(ObjInfoPtr[]);
#define ObjHead(Objs)(struct ObjHead*)(Objs->ObjInfoPtr+Objs->AmountObjInfoPtrs)
};



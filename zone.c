#include <stdint.h>
#include <stdbool.h>
#include <slapi/slapi.h>
#include <slapi/geometry.h>
#include <slapi/transformation.h>
#include <slapi/model/component_definition.h>
#include <slapi/initialize.h>
#include <slapi/model/model.h>
#include <slapi/model/entities.h>
#include <slapi/model/face.h>
#include <slapi/model/edge.h>
#include <slapi/model/vertex.h>
#include <SketchUpAPI/model/geometry_input.h>
#include <SketchUpAPI/geometry/transformation.h>
#include <assert.h>
#include <D3D9Types.h>
#include <stdio.h>
#include <excpt.h>
void DecompressImage(unsigned char* rgba, int width, int height, void const* blocks, int flags);
#define a(function,...)(function(__VA_ARGS__))
#define b(function,...)(assert(function(__VA_ARGS__)==SU_ERROR_NONE))
void*MeshesObjects(a)
    struct Meshes *a;
{
    struct RoomMesh(*currentRoom);
    struct Mesh(*currentMesh);
#define meshpointer __based(currentMesh)
#define zonepointer
#include "GameFormatZone.h"
    size_t currentRoomIndex=0;
    for(currentRoom=&a->MeshRooms;currentRoomIndex<a->AmountRooms;++currentRoomIndex,currentMesh=&currentRoom->Room,
            currentRoom=currentMesh->next);
    return currentRoom;
}

fillDefinitions(modelentities,Definitions,ZoneBuffer)
    SUEntitiesRef modelentities;
    SUComponentDefinitionRef(Definitions)[];
    struct Zone(*ZoneBuffer);
{
    struct Mesh(*currentMesh);
#undef zonepointer
#undef MeshesObjects
#define zonepointer __based(ZoneBuffer)
#include "GameFormatZone.h"
	size_t offsetData = 0;
	SUTextureRef* textures = _alloca(ZoneBuffer->TextPtr->AmountTextHead * sizeof(SUTextureRef));
	for (unsigned int i = 0; i < ZoneBuffer->TextPtr->AmountTextHead; offsetData += i[textureheaders(*ZoneBuffer->TextPtr)].datasize, ++i)
	{
		SUImageRepRef currentimagerep = SU_INVALID;
		b(SUImageRepCreate, &currentimagerep);

		printf("width: %d, height: %d\n", i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize);

		char* outbuffer = malloc(i[textureheaders(*ZoneBuffer->TextPtr)].xsize * i[textureheaders(*ZoneBuffer->TextPtr)].ysize * 4);

		DecompressImage(outbuffer, i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize, texturedata(*ZoneBuffer->TextPtr) + offsetData,
			*(unsigned long*)i[textureheaders(*ZoneBuffer->TextPtr)].format == *(unsigned long*)"DXT1" ? 1 : 2);
		b(SUImageRepSetData, currentimagerep, i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize, 32, 0, 
			*(unsigned long*)i[textureheaders(*ZoneBuffer->TextPtr)].format == 0x15 ? texturedata(*ZoneBuffer->TextPtr) + offsetData : outbuffer);
		
		//printfrawbytes(outbuffer, i[textureheaders(*ZoneBuffer->TextPtr)].xsize * i[textureheaders(*ZoneBuffer->TextPtr)].ysize * 4);

		//system("PAUSE");

		i[textures] = (SUTextureRef)SU_INVALID;

		b(SUTextureCreateFromImageRep, textures + i, currentimagerep);

		free(outbuffer);
	}
	SUMaterialRef* material = _alloca(ZoneBuffer->TextPtr->AmountTextInfo * sizeof(SUMaterialRef));
	printf("%d\n", ZoneBuffer->TextPtr->AmountTextInfo);
	for (unsigned int i = 0; i < ZoneBuffer->TextPtr->AmountTextInfo; ++i)
		i[material] = (SUMaterialRef)SU_INVALID, b(SUMaterialCreate, material + i), 
		i[ZoneBuffer->TextPtr->textureinfo].primarytexture != -1 ? b(SUMaterialSetTexture, i[material], (i[ZoneBuffer->TextPtr->textureinfo].primarytexture)[textures]) : 0;
    typeMeshesObjects*object=MeshesObjects(ZoneBuffer->MshPtr);
    size_t currentIndex=0;
    for(currentMesh=&object->Objects;currentIndex<object->AmountObjects;++currentIndex,currentMesh=currentMesh->next)
    {
        SUEntitiesRef entities=SU_INVALID;
		
        b(SUComponentDefinitionGetEntities,currentIndex[Definitions],&entities);
        SUGeometryInputRef geom_input=SU_INVALID;
        b(SUGeometryInputCreate,&geom_input);
        struct SUPoint3D *vertices=_alloca(currentMesh->AmountVertices*sizeof *vertices);
        for(size_t i=0;i<currentMesh->AmountVertices;++i)
        {
            //struct SUPoint3D vertices;
            vertices[i].x=currentMesh->vertexData[i].X,
            vertices[i].y=currentMesh->vertexData[i].Y,
            vertices[i].z=currentMesh->vertexData[i].Z;
            //b(SUGeometryInputAddVertex,geom_input,&vertices);
        }
        b(SUGeometryInputSetVertices,geom_input,currentMesh->AmountVertices,vertices);
        SUFaceRef *face = _alloca(sizeof(*face)*currentMesh->AmountGroups);
        for(size_t i=0;i<currentMesh->AmountGroups;++i)
        { 
            /*switch(i[groupData(*currentMesh)].PrimitiveType)
            {
                case D3DPT_TRIANGLESTRIP: 
                {*/
			unsigned long amountfaces = 0;
                    for(unsigned long(y)=i[groupData(*currentMesh)].StartIndex;
						amountfaces < i[groupData(*currentMesh)].NumFaces;
                        ++y)
                    {
						SULoopInputRef loop_input = SU_INVALID;
						b(SULoopInputCreate, &loop_input);
                        /*bool Continue=false;
                        for(unsigned long(z)=i[groupData(*currentMesh)].StartIndex;z<y;++z)
                            if(y[indexData(*currentMesh)]==z[indexData(*currentMesh)])
                                Continue=true;
                        if(!Continue)*/

						unsigned long z = y;

#define getindex(x) (x < currentMesh->AmountIndexes ? (x)[indexData(*currentMesh)] : (x - currentMesh->AmountIndexes) [indexData(*currentMesh)])

						for (; !(getindex(z) != getindex(z + 1)
							&& getindex(z) != getindex(z + 2) &&
							getindex(z + 1) != getindex(z + 2)); ++z);

						struct SUMaterialInput mat_input;

						mat_input.material = i[groupData(*currentMesh)].MaterIndex[material];

						mat_input.num_uv_coords = 3;

						unsigned long currentindex;
							for(unsigned long x = z; x < 3 + z; ++x)
								currentindex = getindex(x),
								assert(currentindex < currentMesh->AmountVertices),
								printf("%d\n", currentindex),
								(x-z)[mat_input.vertex_indices] = currentindex,
								(x - z)[mat_input.uv_coords].x = currentindex[currentMesh->vertexData].U,
								(x - z)[mat_input.uv_coords].y = currentindex[currentMesh->vertexData].V,
								b(SULoopInputAddVertexIndex, loop_input, currentindex);

							//SU_ERROR_UNSUPPORTED

							size_t face_index;

							b(SUGeometryInputAddFace, geom_input, &loop_input, &face_index);

							b(SUGeometryInputFaceSetFrontMaterial, geom_input, face_index, &mat_input);

							b(SUGeometryInputFaceSetBackMaterial, geom_input, face_index, &mat_input);

							++amountfaces;
                        //printf("%d\n",y[indexData(*currentMesh)]);
                    }
					b(SUEntitiesFill, entities, geom_input, true);
                    /*SUFaceRef(face) =SU_INVALID;
                        if(a(SUFaceCreate,&face, vertices, &loop_input)==SU_ERROR_NONE)
                            //assert(0),
                            printf("seriosuy"),
                            b(SUEntitiesAddFaces,entities, 1, &face),
                            b(SULoopInputCreate,&loop_input
                    b(SUEntitiesFill,entities,geom_input,true);
              /*  }
                break;
            }*/
        }
        //b(SUEntitiesAddFaces,entities, currentMesh->AmountGroups, face);
    }   
}

drawObject(modelentities,Definitions,ZoneBuffer,Transformation,ObjectIndex)
    SUEntitiesRef modelentities;
    SUComponentDefinitionRef(Definitions)[];
    struct Zone(*ZoneBuffer);
    struct FloatingPosition Transformation[];
    unsigned short ObjectIndex;
{
#undef meshpointer
#define meshpointer
#undef MeshesObjects
#include "GameFormatZone.h"
#define rmxpointer
#define roompointer
#include "GameFormatRMX.h"
    assert(ObjectIndex<ZoneBuffer->ObjsPtr->AmountObjInfoPtrs);
    if(ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex]==-1)
        return; //could possibly load another zone file and search there
    DWORD*Code=ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex]->ObjectCodeBin;
    struct SCOPE {struct Transformations{struct {float x,z,y;} Transformation, Rotation;} *Transformations;
                    struct SCOPE *previous, *next;} DefaultScope={&(struct Transformations){0}},*CurrentScope=&DefaultScope;
    while(*Code!=0xFFFFFFFFul)
    {
        if(*(Code)==0xFFFFFFFEul&&++Code)
        {
            CurrentScope->next=malloc(sizeof*CurrentScope),
            CurrentScope->next->previous=CurrentScope,
            CurrentScope=CurrentScope->next,
            CurrentScope->Transformations=&(struct Transformations){0};
        }
        else if(*(Code)==0xFFFFFFFDul&&++Code)
        {
            CurrentScope=CurrentScope->previous,
            free(CurrentScope->next);
        }
        else if(*(Code)==0xFFFFFFFCul&&++Code) CurrentScope->Transformations=Code,
            ++*(struct Transformations **)&Code;
        else
        {
             SUComponentInstanceRef instance;
             b(SUComponentDefinitionCreateInstance,Definitions[*Code],&instance);

			 //unsigned long objecthash = (*Code)[ObjHead(ZoneBuffer->ObjsPtr)].ObjHash;

			 char name[0x20];

			 sprintf(name, "%u_%u", ObjectIndex, *Code/*, 0[(*Code)[ObjHead(ZoneBuffer->ObjsPtr)].unknown2c]++*/);
			 //printf("%x", objecthash);
			 //system("PAUSE");
			 b(SUComponentInstanceSetName, instance, name);

             //SUEntitiesRef entities=SU_INVALID;
             //b(SUComponentDefinitionGetEntities,Definitions[*Code],&entities);

             static struct SUTransformation matrix,matrixtransform,matrixrotation;

             struct SUPoint3D transformation, rotation;

             for(size_t(u)=0;u<3;++u)
                u[(double*)&transformation]=u[(float*)CurrentScope->Transformations]+u[(float*)Transformation];

			 /*double ugly = transformation.y;

			 transformation.y = transformation.z;

			 transformation.z = ugly;*/

            for(size_t(u)=0;u<3;++u)
                u[(double*)&rotation]=u[(float*)&CurrentScope->Transformations->Rotation]+u[(float*)(Transformation+1)];

            /*for(size_t(u)=0;u<sizeof(matrixtransform.values)/sizeof(double);++u)
                matrixtransform.values[u]=matrixrotation.values[u]=1.0;*/

			/*for (size_t(u) = 0; u < 3; ++u)
				matrixtransform.values[u * 4 + 3] = u[(float*)CurrentScope->Transformations] + u[(float*)Transformation];


			matrixtransform.values[15] = 1.0;*/
            b(SUTransformationTranslation,&matrixtransform,&transformation),
            //b(SUTransformationRotation,&matrixrotation,&transformation,&rotation,3.14f/180.0f),
            //b(SUTransformationMultiply,&matrixtransform,&matrixrotation,&matrix),

             b(SUComponentInstanceSetTransform,instance,&matrixtransform),
             //b(SUComponentInstanceSetTransform,instance,&matrixtransform),

             b(SUEntitiesAddInstance,modelentities,instance,NULL);
             char buff[sizeof("instance_xxxxx.skp")];
             a(sprintf,buff,"instance_%d.skp",ObjectIndex),
             b(SUComponentInstanceSaveAs,instance,buff),
             //printf("instance\n");

             Code+=4;
        }
    }
}
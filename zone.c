#include <stdint.h>
#include <stdbool.h>
#include <SketchUpAPI/slapi.h>
#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/transformation.h>
#include <SketchUpAPI/model/component_definition.h>
#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/geometry_input.h>
#include <SketchUpAPI/geometry/transformation.h>
#include <assert.h>
#include <D3D9Types.h>
#include <stdio.h>
#include <excpt.h>
#define _USE_MATH_DEFINES
#include <math.h>
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

struct SUTransformation* get_transform(matrix, rotation, transformation)
    struct SUTransformation* matrix;
struct SUVector3D* transformation;
struct SUPoint3D *rotation;
{
    struct SUTransformation matrixtransform = {0}, matrixrotationX = { 0 }, matrixrotationY = { 0 },
        matrixrotationZ = { 0 }, matrixrotation = { 0 };

    struct SUTransformation* outmatrix = NULL;
    int transform_needed = 0, rotation_needed = 0;

    for (size_t(u) = 0; u < 3; ++u)
        transform_needed |= !!(u[(double*)transformation]);// + u[(float*)Transformation];

    /*double ugly = transformation.y;

    transformation.y = transformation.z;

    transformation.z = ugly;*/

    for (size_t(u) = 0; u < 3; ++u)
        rotation_needed |= !!(u[(double*)rotation]);// + u[(float*)(Transformation + 1)];

    /*for(size_t(u)=0;u<sizeof(matrixtransform.values)/sizeof(double);++u)
        matrixtransform.values[u]=matrixrotation.values[u]=1.0;*/

        /*for (size_t(u) = 0; u < 3; ++u)
            matrixtransform.values[u * 4 + 3] = u[(float*)CurrentScope->Transformations] + u[(float*)Transformation];


        matrixtransform.values[15] = 1.0;*/
    /*if (transform_needed) {
        b(SUTransformationTranslation, matrixtransform, (struct SUVector3D[1]) { {transformation->x, transformation->y, transformation->z} });
        //memcpy(matrix->values + 3 * 4, matrixtransform->values + 3 * 4, sizeof(double[4]));
    }
    if (rotation_needed, TRUE) {
        b(SUTransformationRotation, matrixrotation, (struct SUPoint3D[1]) { {rotation->y, 0.0, 0.0} }, (struct SUVector3D[1]) { {1.0, 1.0, 1.0} }, M_PI / 180.0f);
        b(SUTransformationRotation, matrixrotation, (struct SUPoint3D[1]) { {0.0, rotation->z, 0.0} }, (struct SUVector3D[1]) { {1.0, 1.0, 1.0} }, M_PI / 180.0f);
        b(SUTransformationRotation, matrixrotation, (struct SUPoint3D[1]) { {0.0, 0.0, rotation->x} }, (struct SUVector3D[1]) { {1.0, 1.0, 1.0} }, M_PI / 180.0f);
        //memcpy(matrix->values, matrixrotation->values, sizeof(double[3][4]));
    }*/
    if (transform_needed || rotation_needed) {
        b(SUTransformationTranslation, &matrixtransform, transformation),
        b(SUTransformationRotation, &matrixrotationX, (struct SUVector3D[]) { 0, }, (struct SUVector3D[]) { 1, 0, 0 }, rotation->x* M_PI / 180.0),
        b(SUTransformationRotation, &matrixrotationY, (struct SUVector3D[]) { 0, }, (struct SUVector3D[]) { 0, 1, 0 }, rotation->y* M_PI / 180.0),
        b(SUTransformationRotation, &matrixrotationZ, (struct SUVector3D[]) { 0, }, (struct SUVector3D[]) { 0, 0, 1 }, rotation->z* M_PI / 180.0);
        b(SUTransformationRotation, &matrixrotation, (struct SUVector3D[]) { 0, }, (struct SUVector3D[]) { 0, 0, 0 }, M_PI / 180.0);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationX, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationY, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationZ, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotation, matrix);
        outmatrix = matrix;
    }
    return outmatrix;
}
fillDefinitions(modelentities, DefinitionsObjects, Definitions,ZoneBuffer)
    SUEntitiesRef modelentities;
    SUComponentDefinitionRef(DefinitionsObjects)[];
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
    //SUComponentDefinitionRef(*Definitions) = _alloca(object->AmountObjects * sizeof * Definitions);
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
    for (size_t ObjectIndex = 0; ObjectIndex < ZoneBuffer->ObjsPtr->AmountObjInfoPtrs; ++ObjectIndex) {
        if (ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex] == -1)
            continue; //could possibly load another zone file and search there

        SUEntitiesRef entities = SU_INVALID;
        b(SUComponentDefinitionGetEntities, DefinitionsObjects[ObjectIndex], &entities);

        struct SUTransformation matrix, * outmatrix;
        struct SUVector3D transformation;
        struct SUPoint3D rotation;
        DWORD* Code = ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex]->ObjectCodeBin;
        struct SCOPE {
             struct xzy{ float x, y, z; } Transformation, Rotation;
            struct SCOPE* previous, * next;
        } DefaultScope = { { 0 } }, * CurrentScope = &DefaultScope;
        while (*Code != 0xFFFFFFFFul)
        {
            if (*(Code) == 0xFFFFFFFEul && ++Code)
            {
                CurrentScope->next = malloc(sizeof * CurrentScope),
                    CurrentScope->next->previous = CurrentScope,
                    CurrentScope = CurrentScope->next,
                    CurrentScope->Transformation = (struct xzy) { 0 };
                CurrentScope->Rotation = (struct xzy){ 0 };
            }
            else if (*(Code) == 0xFFFFFFFDul && ++Code)
            {
                CurrentScope = CurrentScope->previous,
                    free(CurrentScope->next);
            }
            else if (*(Code) == 0xFFFFFFFCul && ++Code)
                CurrentScope->Transformation = (struct xzy){ 0[(struct xzy*)Code].x + CurrentScope->Transformation.x, 0[(struct xzy*)Code].y + CurrentScope->Transformation.y, 0[(struct xzy*)Code].z + CurrentScope->Transformation.z },
                CurrentScope->Rotation = (struct xzy){ 1[(struct xzy*)Code].x + CurrentScope->Rotation.x, 1[(struct xzy*)Code].y + CurrentScope->Rotation.y, 1[(struct xzy*)Code].z + CurrentScope->Rotation.z },
                ++ * (struct xzy(**)[2]) & Code;
            else
            {
                //sprintf(name, "%sObjectDefinition%d", i, ),
                //b(SUComponentDefinitionSetName, Definitions + i, name);
                SUComponentInstanceRef instance = { SU_INVALID };
                b(SUComponentDefinitionCreateInstance, Definitions[*Code], &instance);

                //unsigned long objecthash = (*Code)[ObjHead(ZoneBuffer->ObjsPtr)].ObjHash;

                //char name[0x20];

                //sprintf(name, "%x_%x", ObjectRoomHash, ObjectNodeHash/*, 0[(*Code)[ObjHead(ZoneBuffer->ObjsPtr)].unknown2c]++*/);
                //printf("%x", objecthash);
                //system("PAUSE");
                //b(SUComponentInstanceSetName, instance, name);

                //SUEntitiesRef entities=SU_INVALID;
                //b(SUComponentDefinitionGetEntities,Definitions[*Code],&entities);

                transformation = (struct SUVector3D){ CurrentScope->Transformation.x, CurrentScope->Transformation.y, CurrentScope->Transformation.z};
                rotation = (struct SUPoint3D){ CurrentScope->Rotation.x, CurrentScope->Rotation.y, CurrentScope->Rotation.z };

                
                outmatrix = get_transform(&matrix, &rotation, &transformation);
                if (outmatrix)
                    b(SUComponentInstanceSetTransform, instance, outmatrix);
                    //b(SUComponentInstanceSetTransform,instance,&matrixtransform),

                b(SUEntitiesAddInstance, entities, instance, NULL);
                //char buff[sizeof("instance_xxxxx.skp")];
                //a(sprintf,buff,"instance_%d.skp",ObjectIndex),
                //b(SUComponentInstanceSaveAs,instance,buff),
                //printf("instance\n");

                Code += 4;
            }
        }
    }
}

drawObject(modelentities,Definitions,ZoneBuffer,Transformation,ObjectIndex, ObjectRoomHash, ObjectNodeHash)
    SUEntitiesRef modelentities;
    SUComponentDefinitionRef(Definitions)[];
    struct Zone(*ZoneBuffer);
    struct FloatingPosition Transformation[];
    unsigned short ObjectIndex;
    unsigned int ObjectRoomHash, ObjectNodeHash;
{
#undef meshpointer
#define meshpointer
#undef MeshesObjects
#include "GameFormatZone.h"
#define rmxpointer
#define roompointer
#include "GameFormatRMX.h"

}
#include <stdint.h>
#include <stdbool.h>
#include <SketchUpAPI/model/group.h>
#include <SketchUpAPI/model/material.h>
#include <SketchUpAPI/model/texture.h>
#include <SketchUpAPI/model/image_rep.h>
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
#include <array>
#include "cmn.h"
#include <mutex>


extern "C" {
enum SUResult SUPlane3DSetWithPoints(struct SUPlane3D* plane,
    const struct SUPoint3D* point1,
    const struct SUPoint3D* point2,
    const struct SUPoint3D* point3
);


enum SUResult SUPlane3DIsOn(const struct SUPlane3D* plane,
    const struct SUPoint3D* point,
    bool* is_on
);
}


void DecompressImage(unsigned char* rgba, int width, int height, void const* blocks, int flags);
void*MeshesObjectsf(struct Meshes* a_)
{
    struct RoomMesh(*currentRoom_);
    struct Mesh(*currentMesh_);
#define meshpointer __based(currentMesh_)
#define zonepointer
#include "GameFormatZone.h"
    {
        struct RoomMesh(*&currentRoom) = (struct RoomMesh*&)currentRoom_;
        struct Mesh(*&currentMesh) = (struct Mesh*&)currentMesh_;
        struct Meshes *&a = (struct Meshes*&)a_;
        size_t currentRoomIndex = 0;
        for (currentRoom = (RoomMesh*)&a->MeshRooms;currentRoomIndex < a->AmountRooms;++currentRoomIndex, currentMesh = (Mesh*)&currentRoom->Room,
            currentRoom = (RoomMesh*)currentMesh->next);
        return currentRoom;
    }
}

struct SUTransformation* get_transform(struct SUTransformation* matrix, struct SUVector3D* transformation, struct SUPoint3D* rotation)
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
        b(SUTransformationRotation, &matrixrotationX, std::array<const SUPoint3D, 1>{ { 0, }}.data(), std::array<const SUVector3D, 1>{ {1, 0, 0}}.data(), rotation->x* M_PI / 180.0),
        b(SUTransformationRotation, &matrixrotationY, std::array<const SUPoint3D, 1>{ { 0, }}.data(), std::array<const SUVector3D, 1>{ { 0, 1, 0 }}.data(), rotation->y* M_PI / 180.0),
        b(SUTransformationRotation, &matrixrotationZ, std::array<const SUPoint3D, 1>{ { 0, }}.data(), std::array<const SUVector3D, 1>{ { 0, 0, 1 }}.data(), rotation->z* M_PI / 180.0);
        b(SUTransformationRotation, &matrixrotation, std::array<const SUPoint3D, 1>{ { 0, }}.data(), std::array<const SUVector3D, 1>{ { 0, 0, 0 }}.data(), M_PI / 180.0);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationX, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationY, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotationZ, &matrixtransform);
        b(SUTransformationMultiply, &matrixtransform, &matrixrotation, matrix);
        outmatrix = matrix;
    }
    return outmatrix;
}

/*unsigned long calculate_index(struct Group* groupdata, unsigned short* indexdata, signed long index, unsigned long amountmax)
{
#undef zonepointer
#undef MeshesObjects
#define meshpointer
#define zonepointer
#include "GameFormatZone.h"
    //if (index < groupdata->StartIndex) {
    //    index -= groupdata->StartIndex;
    //    index = abs(index);
    //    index = groupdata->lastindex - (index - 1);
    //}
   // else if (index >= amountmax, false) {
   //     index -= amountmax;
   //     index = (index) + groupdata->StartIndex;
   // }
    return indexdata[index];
}*/

int fillMesh(SUEntitiesRef entities, struct Mesh(*currentMesh_), SUMaterialRef* material)
{
#define meshpointer __based(currentMesh_)
#undef zonepointer
#undef MeshesObjects
#define zonepointer 
#include "GameFormatZone.h"

#define getindex(x) (indexData(*currentMesh)[x])//calculate_index(currgroup, indexData(*currentMesh), x, currentMesh->AmountIndexes))
    {
        struct Mesh(*&currentMesh) = (struct Mesh*&)currentMesh_;
        SUGeometryInputRef geom_input = SU_INVALID;
        b(SUGeometryInputCreate, &geom_input);

        /* ---------- upload vertices only ONCE ---------- */
        struct SUPoint3D* vertices = (decltype(vertices))malloc(currentMesh->AmountVertices * sizeof * vertices);
        for (size_t i = 0; i < currentMesh->AmountVertices; ++i) {
            vertices[i].x = currentMesh->vertexData[i].X;
            vertices[i].y = currentMesh->vertexData[i].Y;
            vertices[i].z = currentMesh->vertexData[i].Z;
        }
        b(SUGeometryInputSetVertices, geom_input, currentMesh->AmountVertices, vertices);

        /* ---------- process all groups, accumulate faces ---------- */
        for (size_t g = 0; g < currentMesh->AmountGroups; ++g) {
            struct Group* currgroup = groupData(*currentMesh);
            //assert(g[currgroup].PrimitiveType == D3DPT_TRIANGLESTRIP);

            size_t pos = currgroup[g].StartIndex;
            size_t remaining = currgroup[g].NumFaces;

            while (remaining > 0) {
                signed long raw0 = getindex(pos);
                signed long raw1 = getindex((pos + 1));
                signed long raw2 = getindex((pos + 2));

                /* Validate all three base indices */
                bool validTriangle =
                    (raw0 >= 0 && (size_t)raw0 < currentMesh->AmountVertices) &&
                    (raw1 >= 0 && (size_t)raw1 < currentMesh->AmountVertices) &&
                    (raw2 >= 0 && (size_t)raw2 < currentMesh->AmountVertices);

                if (!validTriangle) {
                    printf("INVALID INDEX in triangle: %ld, %ld, %ld – skipping\n",
                        raw0, raw1, raw2);
                    pos += 1;
                    remaining -= 1;
                    continue;
                }

                size_t vi0 = (size_t)raw0;
                size_t vi1 = (size_t)raw1;
                size_t vi2 = (size_t)raw2;

                bool nonDegenerate = (vi0 != vi1) && (vi1 != vi2) && (vi0 != vi2);

                /* Quad check */
                bool canQuad = false;
                signed long raw3 = -1;
                size_t vi3 = 0;
                if (remaining >= 2 && nonDegenerate) {
                    raw3 = getindex(pos + 3);
                    if (raw3 >= 0 && (size_t)raw3 < currentMesh->AmountVertices) {
                        vi3 = (size_t)raw3;
                        if (vi3 != vi0 && vi3 != vi1 && vi3 != vi2) {
                            struct SUPlane3D plane = { 0 };
                            b(SUPlane3DSetWithPoints, &plane,
                                &vertices[vi0], &vertices[vi1], &vertices[vi2]);
                            bool is_on;
                            b(SUPlane3DIsOn, &plane, &vertices[vi3], &is_on);
                            canQuad = is_on;
                        }
                    }
                }

                if (nonDegenerate) {
                    SULoopInputRef loop_input = SU_INVALID;
                    b(SULoopInputCreate, &loop_input);

                    struct SUMaterialInput mat_input;
                    /* USE THE SINGLE MATERIAL POINTER CORRECTLY */
                    SUGeometryInputRef geom_input = SU_INVALID;
                    b(SUGeometryInputCreate, &geom_input);

                    /* ---------- upload vertices only ONCE ---------- */
                    struct SUPoint3D* vertices = (struct SUPoint3D *)malloc(currentMesh->AmountVertices * sizeof * vertices);
                    for (size_t i = 0; i < currentMesh->AmountVertices; ++i) {
                        vertices[i].x = currentMesh->vertexData[i].X;
                        vertices[i].y = currentMesh->vertexData[i].Y;
                        vertices[i].z = currentMesh->vertexData[i].Z;
                    }
                    b(SUGeometryInputSetVertices, geom_input, currentMesh->AmountVertices, vertices);



                    /* ---------- process all groups, accumulate faces ---------- */
                    for (size_t g = 0; g < currentMesh->AmountGroups; ++g) {
                        struct Group* currgroup = groupData(*currentMesh);
                        assert(g[currgroup].PrimitiveType == D3DPT_TRIANGLESTRIP);

                        /* Validate material once */
                        SUMaterialRef mat = SU_INVALID;
                        if (material != NULL) {
                            mat = g[currgroup].MaterIndex[material];
                            if (!SUIsValid(mat)) {
                                printf("WARNING: material is invalid (ptr %p, value %p). Faces will have no material.\n",
                                    (void*)material, mat.ptr);
                                SUSetInvalid(mat);
                            }
                        }
                        else {
                            printf("WARNING: material pointer is NULL. Faces will have no material.\n");
                        }

                        size_t pos = currgroup[g].StartIndex;
                        size_t remaining = currgroup[g].NumFaces;

                        while (remaining > 0) {
                            signed long raw0 = getindex(pos);
                            signed long raw1 = getindex(pos + 1);
                            signed long raw2 = getindex(pos + 2);

                            bool validTriangle =
                                (raw0 >= 0 && (size_t)raw0 < currentMesh->AmountVertices) &&
                                (raw1 >= 0 && (size_t)raw1 < currentMesh->AmountVertices) &&
                                (raw2 >= 0 && (size_t)raw2 < currentMesh->AmountVertices);

                            if (!validTriangle) {
                                printf("INVALID INDEX in triangle: %ld, %ld, %ld – skipping\n",
                                    raw0, raw1, raw2);
                                pos += 1;
                                remaining -= 1;
                                continue;
                            }

                            size_t vi0 = (size_t)raw0;
                            size_t vi1 = (size_t)raw1;
                            size_t vi2 = (size_t)raw2;

                            bool nonDegenerate = (vi0 != vi1) && (vi1 != vi2) && (vi0 != vi2);

                            /* Quad check */
                            bool canQuad = false;
                            signed long raw3 = -1;
                            size_t vi3 = 0;
                            if (remaining >= 2 && nonDegenerate) {
                                raw3 = getindex(pos + 3);
                                if (raw3 >= 0 && (size_t)raw3 < currentMesh->AmountVertices) {
                                    vi3 = (size_t)raw3;
                                    if (vi3 != vi0 && vi3 != vi1 && vi3 != vi2) {
                                        struct SUPlane3D plane = { 0 };
                                        b(SUPlane3DSetWithPoints, &plane,
                                            &vertices[vi0], &vertices[vi1], &vertices[vi2]);
                                        bool is_on;
                                        b(SUPlane3DIsOn, &plane, &vertices[vi3], &is_on);
                                        canQuad = is_on;
                                    }
                                }
                            }

                            if (nonDegenerate) {
                                SULoopInputRef loop_input = SU_INVALID;
                                b(SULoopInputCreate, &loop_input);

                                struct SUMaterialInput mat_input;
                                mat_input.material = mat;   /* use the validated material (or SU_INVALID) */
                                size_t face_index;

                                if (canQuad) {
                                    mat_input.num_uv_coords = 4;
                                    size_t order[4] = { vi0, vi1, vi3, vi2 };
                                    printf("QUAD: indices = %zu, %zu, %zu, %zu\n",
                                        order[0], order[1], order[2], order[3]);
                                    for (int v = 0; v < 4; ++v) {
                                        size_t idx = order[v];
                                        mat_input.vertex_indices[v] = idx;
                                        mat_input.uv_coords[v].x = currentMesh->vertexData[idx].U;
                                        mat_input.uv_coords[v].y = currentMesh->vertexData[idx].V;
                                        b(SULoopInputAddVertexIndex, loop_input, idx);
                                    }
                                    pos += 2;
                                    remaining -= 2;
                                }
                                else {
                                    mat_input.num_uv_coords = 3;
                                    size_t order[3] = { vi0, vi1, vi2 };
                                    printf("TRI:  indices = %zu, %zu, %zu\n",
                                        order[0], order[1], order[2]);
                                    for (int v = 0; v < 3; ++v) {
                                        size_t idx = order[v];
                                        mat_input.vertex_indices[v] = idx;
                                        mat_input.uv_coords[v].x = currentMesh->vertexData[idx].U;
                                        mat_input.uv_coords[v].y = currentMesh->vertexData[idx].V;
                                        b(SULoopInputAddVertexIndex, loop_input, idx);
                                    }
                                    pos += 1;
                                    remaining -= 1;
                                }

                                b(SUGeometryInputAddFace, geom_input, &loop_input, &face_index);

                                /* Only set material if we have a valid one */
                                if (!SUIsInvalid(mat)) {
                                    b(SUGeometryInputFaceSetFrontMaterial, geom_input, face_index, &mat_input);
                                    b(SUGeometryInputFaceSetBackMaterial, geom_input, face_index, &mat_input);
                                }
                            }
                            else {
                                printf("DEGENERATE skip: %zu, %zu, %zu\n", vi0, vi1, vi2);
                                pos += 1;
                                remaining -= 1;
                            }
                        }
                    }
                    static std::mutex enflm;
                    {
                        std::unique_lock lk(enflm);
                        /* ---------- fill entities ONCE (now with valid material) ---------- */
                         /* ---------- fill entities ONCE ---------- */
                        b(SUEntitiesFill, entities, geom_input, true);
                        free(vertices);   /* was: g[currgroup].MaterIndex[material] */
                        return 0;
                    }
                   
                    size_t face_index;

                    if (canQuad) {
                        mat_input.num_uv_coords = 4;
                        size_t order[4] = { vi0, vi1, vi3, vi2 };
                        printf("QUAD: indices = %zu, %zu, %zu, %zu\n",
                            order[0], order[1], order[2], order[3]);
                        for (int v = 0; v < 4; ++v) {
                            size_t idx = order[v];
                            mat_input.vertex_indices[v] = idx;
                            mat_input.uv_coords[v].x = currentMesh->vertexData[idx].U;
                            mat_input.uv_coords[v].y = currentMesh->vertexData[idx].V;
                            b(SULoopInputAddVertexIndex, loop_input, idx);
                        }
                        pos += 2;
                        remaining -= 2;
                    }
                    else {
                        mat_input.num_uv_coords = 3;
                        size_t order[3] = { vi0, vi1, vi2 };
                        printf("TRI:  indices = %zu, %zu, %zu\n",
                            order[0], order[1], order[2]);
                        for (int v = 0; v < 3; ++v) {
                            size_t idx = order[v];
                            mat_input.vertex_indices[v] = idx;
                            mat_input.uv_coords[v].x = currentMesh->vertexData[idx].U;
                            mat_input.uv_coords[v].y = currentMesh->vertexData[idx].V;
                            b(SULoopInputAddVertexIndex, loop_input, idx);
                        }
                        pos += 1;
                        remaining -= 1;
                    }

                    b(SUGeometryInputAddFace, geom_input, &loop_input, &face_index);
                    //b(SUGeometryInputFaceSetFrontMaterial, geom_input, face_index, &mat_input);
                   // b(SUGeometryInputFaceSetBackMaterial, geom_input, face_index, &mat_input);
                }
                else {
                    printf("DEGENERATE skip: %zu, %zu, %zu\n", vi0, vi1, vi2);
                    pos += 1;
                    remaining -= 1;
                }
            }
        }
        //static std::mutex enflm;
        //{
        //    std::unique_lock lk(enflm);
            /* ---------- fill entities ONCE (now with valid material) ---------- */
       //     b(SUEntitiesFill, entities, geom_input, true);
       // }
        free(vertices);
    }
}

int fillMaterial(SUMaterialRef* material, struct Zone(*ZoneBuffer_))
{
#define meshpointer
#undef zonepointer
#undef MeshesObjects
#define zonepointer __based(ZoneBuffer_)
#include "GameFormatZone.h"
    {
        struct Zone(*&ZoneBuffer) = (struct Zone*&)ZoneBuffer_;

        size_t offsetData = 0;
        SUTextureRef* textures = (SUTextureRef*)malloc(ZoneBuffer->TextPtr->AmountTextHead * sizeof(SUTextureRef));
        for (unsigned int i = 0; i < ZoneBuffer->TextPtr->AmountTextHead; offsetData += i[textureheaders(*ZoneBuffer->TextPtr)].datasize, ++i)
        {
            SUImageRepRef currentimagerep = SU_INVALID;
            b(SUImageRepCreate, &currentimagerep);

            printf("width: %d, height: %d\n", i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize);

            char* outbuffer = (char*)malloc(i[textureheaders(*ZoneBuffer->TextPtr)].xsize * i[textureheaders(*ZoneBuffer->TextPtr)].ysize * 4);

            DecompressImage((unsigned char*)outbuffer, i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize, texturedata(*ZoneBuffer->TextPtr) + offsetData,
                *(unsigned long*)i[textureheaders(*ZoneBuffer->TextPtr)].format == *(unsigned long*)"DXT1" ? 1 : 2);
            b(SUImageRepSetData, currentimagerep, i[textureheaders(*ZoneBuffer->TextPtr)].xsize, i[textureheaders(*ZoneBuffer->TextPtr)].ysize, 32, 0,
                (const SUByte*)(*(unsigned long*)i[textureheaders(*ZoneBuffer->TextPtr)].format == 0x15 ? texturedata(*ZoneBuffer->TextPtr) + offsetData : outbuffer));

            //printfrawbytes(outbuffer, i[textureheaders(*ZoneBuffer->TextPtr)].xsize * i[textureheaders(*ZoneBuffer->TextPtr)].ysize * 4);

            //system("PAUSE");

            SUSetInvalid(i[textures]);

            b(SUTextureCreateFromImageRep, textures + i, currentimagerep);

            free(outbuffer);
        }
        printf("%d\n", ZoneBuffer->TextPtr->AmountTextInfo);
        for (unsigned int i = 0; i < ZoneBuffer->TextPtr->AmountTextInfo; ++i)
            SUSetInvalid(i[material]), b(SUMaterialCreate, material + i),
            i[ZoneBuffer->TextPtr->textureinfo].primarytexture != -1 ? b(SUMaterialSetTexture, i[material], (i[ZoneBuffer->TextPtr->textureinfo].primarytexture)[textures]) : 0;
        free(textures);
    }
}

int fillDefinitions(SUEntitiesRef modelentitie, SUComponentDefinitionRef(DefinitionsObjects)[], SUComponentDefinitionRef(Definitions)[], struct Zone(*ZoneBuffer_), SUMaterialRef* material)
{
    struct Mesh(*currentMesh_);
#define meshpointer __based(currentMesh_)
#undef zonepointer
#undef MeshesObjects
#define zonepointer __based(ZoneBuffer_)
#include "GameFormatZone.h"
    {
        struct Zone(*&ZoneBuffer) = (struct Zone*&)ZoneBuffer_;
        struct Mesh(*&currentMesh) = (struct Mesh*&)currentMesh_;
        typeMeshesObjects* object = MeshesObjects(ZoneBuffer->MshPtr);
        //SUComponentDefinitionRef(*Definitions) = _alloca(object->AmountObjects * sizeof * Definitions);
        size_t currentIndex = 0;
        for (currentMesh = (Mesh*)&object->Objects;currentIndex < object->AmountObjects;++currentIndex, currentMesh = (Mesh*)currentMesh->next)
        {
            SUEntitiesRef entities = SU_INVALID;
            b(SUComponentDefinitionGetEntities, currentIndex[Definitions], &entities);
            fillMesh(entities, (::Mesh*)currentMesh, material);
        }
        for (size_t ObjectIndex = 0; ObjectIndex < ZoneBuffer->ObjsPtr->AmountObjInfoPtrs; ++ObjectIndex) {
            if ((unsigned int)ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex] == -1)
                continue; //could possibly load another zone file and search there

            SUEntitiesRef entities = SU_INVALID;
            b(SUComponentDefinitionGetEntities, DefinitionsObjects[ObjectIndex], &entities);

            struct SUTransformation matrix, * outmatrix;
            struct SUVector3D transformation;
            struct SUPoint3D rotation;
            DWORD* Code = ZoneBuffer->ObjsPtr->ObjInfoPtr[ObjectIndex]->ObjectCodeBin;
            struct xzy { float x, y, z; };
            struct SCOPE {
                struct { float x, y, z; };
                SUGroupRef group;
                struct SCOPE* previous, * next;
            } DefaultScope = { { SU_INVALID } }, * CurrentScope = &DefaultScope;
            b(SUGroupCreate, &CurrentScope->group);
            while (*Code != 0xFFFFFFFFul)
            {
                if (*(Code) == 0xFFFFFFFEul && ++Code)
                {
                    CurrentScope->next = (SCOPE*)malloc(sizeof * CurrentScope),
                        CurrentScope->next->previous = CurrentScope,
                        CurrentScope = CurrentScope->next,
                        SUSetInvalid(CurrentScope->group);
                    b(SUGroupCreate, &CurrentScope->group);
                }
                else if (*(Code) == 0xFFFFFFFDul && ++Code)
                {
                    CurrentScope = CurrentScope->previous;
                    SUEntitiesAddGroup(entities, CurrentScope->next->group);
                    free(CurrentScope->next);
                }
                else if (*(Code) == 0xFFFFFFFCul && ++Code) {
                    struct xzy Transformation = 0[(struct xzy*)Code], Rotation = 1[(struct xzy*)Code];
                    rotation =  { Transformation.x, Transformation.y, Transformation.z };
                    transformation = { Rotation.x, Rotation.y, Rotation.z };
                    outmatrix = get_transform(&matrix, &transformation, &rotation);
                    if (outmatrix)
                        b(SUGroupSetTransform, CurrentScope->group, outmatrix);
                    ++*(struct xzy(**)[2]) & Code;
                }
                else
                {
                    SUEntitiesRef entities = SU_INVALID;
                    b(SUGroupGetEntities, CurrentScope->group, &entities);
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

                    //b(SUComponentInstanceSetTransform,instance,&matrixtransform),

                    b(SUEntitiesAddInstance, entities, instance, NULL);
                    //char buff[sizeof("instance_xxxxx.skp")];
                    //a(sprintf,buff,"instance_%d.skp",ObjectIndex),
                    //b(SUComponentInstanceSaveAs,instance,buff),
                    //printf("instance\n");

                    Code += 4;
                }
            }
            SUEntitiesAddGroup(entities, CurrentScope->group);
        }
    }
}


int findroomindexbyhash(struct Zone(*ZoneBuffer_), unsigned int hash)
{
    struct Mesh;
    struct Mesh(*currentMesh_);
#define meshpointer __based(currentMesh_)
#undef zonepointer
#undef MeshesObjects
#define zonepointer __based(ZoneBuffer_)
#include "GameFormatZone.h"
    {
        struct Mesh(*&currentMesh) = (struct Mesh*&)currentMesh_;
        struct Zone(*&ZoneBuffer) = (struct Zone*&)ZoneBuffer_;
        currentMesh = &ZoneBuffer->MshPtr->MeshRooms.Room;
        struct RoomMesh* currroomz = &ZoneBuffer->MshPtr->MeshRooms;
        for (size_t(i) = 0; i < ZoneBuffer->MshPtr->AmountRooms; ++i, currentMesh = (struct Mesh*)currentMesh->next,
            currroomz = (struct RoomMesh*)currentMesh,
            currentMesh = &currroomz->Room)
            if (currroomz->RoomHash == hash)
                return i;
        return -1;
    }
}

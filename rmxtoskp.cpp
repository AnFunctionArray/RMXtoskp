#include <stdio.h>
#include <stdbool.h>
#include <SketchUpAPI/slapi.h>
#include <SketchUpAPI/geometry.h>
#include <SketchUpAPI/initialize.h>
#include <SketchUpAPI/model/model.h>
#include <SketchUpAPI/model/entities.h>
#include <SketchUpAPI/model/face.h>
#include <SketchUpAPI/model/edge.h>
#include <SketchUpAPI/model/vertex.h>
#include <SketchUpAPI/model/component_instance.h>
#include <SketchUpAPI/geometry/transformation.h>
#include <SketchUpAPI/model/component_definition.h>
#include <assert.h>
#include <ppl.h>

#include <math.h>
#include <array>
//extern "C" {
#include "cmn.h"



size_t
    initFile(FILE(**file), char(**fileBuffer), char* filename, char* access)
{
    *file = a(fopen, (filename), access);
    a(fseek, *file, 0, SEEK_END);
    size_t(fileSize) = a(ftell, *file);
    a(fseek, *file, 0, SEEK_SET),
        * fileBuffer = (char*)a(malloc, fileSize),
        a(fread, *fileBuffer, sizeof(char), fileSize, *file);
    return fileSize;
}
/* D. J. Bernstein hash function */
static unsigned int djb_hash(const char* cp)
{
    unsigned int hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char)*cp++;
    return hash;
}
#include <math.h>

struct rots {
    double y, x, z;
};

struct rots extractEulerAngles(double(*matrix)[4]) {
    double pitch, roll, yaw;

    // Extract rotation values
    pitch = asin(matrix[2][0]);
    if (fabs(cos(pitch)) > 0.0001) {
        roll = atan2(-matrix[2][1], matrix[2][2]);
        yaw = atan2(-matrix[1][0], matrix[0][0]);
    }
    else {
        // Gimbal lock case
        roll = 0.0;
        yaw = atan2(matrix[1][2], matrix[1][1]);
    }

    return rots{ pitch * (180 / M_PI), roll * (180 / M_PI), yaw * (180 / M_PI) };
}
int
((main_c)(int number, char(**arguments))) //grand entry

{ //ugly braces
    //read the file maybe
    bool wearewriting = false;
    FILE(*RMXFile), * ZoneFile;
    struct RMX(*RMXBuffer_);
    struct Room(*currentRoom_);
    struct Zone(*ZoneBuffer_);
    size_t szRMXFile;
    szRMXFile = initFile(&RMXFile, (char**)&RMXBuffer_, 1[arguments], (char*)"r+b");
    initFile(&ZoneFile, (char**)&ZoneBuffer_, 2[arguments], (char*)"r+b");

    struct Mesh(*currentMesh_);
    //It is not uncommon to include a header file mid function definition
    //It is also equally common to specify address base
#define rmxpointer __based(RMXBuffer_)
#define roompointer __based(currentRoom_)
#include "GameFormatRMX.h"
#define meshpointer __based(currentMesh_)
#define zonepointer __based(ZoneBuffer_)
#include "GameFormatZone.h"
    {
        struct Mesh(*&currentMesh) = (struct Mesh*&)currentMesh_;
        struct RMX(*&RMXBuffer) = (struct RMX*&)RMXBuffer_;
        struct Room(*&currentRoom) = (struct Room*&)currentRoom_;
        struct Zone(*&ZoneBuffer) = (struct Zone*&)ZoneBuffer_;
        SUInitialize();

        if (number > 3)
        {
            //SUInitialize();
            SUModelRef model = SU_INVALID;

            b(SUModelCreateFromFile, &model, 3[arguments]);

            SUEntitiesRef entities = SU_INVALID;
            b(SUModelGetEntities, model, &entities);

            SUComponentInstanceRef instances[260];

            size_t szinstances;

            b(SUEntitiesGetInstances, entities, 260, instances, &szinstances);

            while (szinstances--)
            {
                struct SUTransformation matrix;
                b(SUComponentInstanceGetTransform, szinstances[instances], &matrix);

                struct FloatingPosition pos = { .x = (float)matrix.values[12], .y = (float)matrix.values[13], .z = (float)matrix.values[14],  .d = (float)matrix.values[15], };

                SUStringRef name = SU_INVALID; char szname[100];
                char sznametoinsert[100];

                b(SUStringCreate, &name);

                b(SUComponentInstanceGetName, szinstances[instances], &name);

                b(SUStringGetUTF8, name, sizeof szname, szname, std::array<size_t, 1> { 0 }.data());
                
                struct NL_OBJ* node;

                if (strchr(szname, ':')) {
                    unsigned long objecttoinherithash, roomtoniheritobjfromhash;
                    unsigned long roomhashtarget;
                        
                    int nread = sscanf(szname, "%8x_%8x:%8x_%s", &roomtoniheritobjfromhash, &objecttoinherithash, &roomhashtarget, sznametoinsert);
                    assert(nread >= 4);
                    node = (NL_OBJ*)addNode(&szRMXFile, (::RMX**)&RMXBuffer, objecttoinherithash, roomtoniheritobjfromhash, roomhashtarget);
                    node->ObjectHash = djb_hash(sznametoinsert);
                }
                else {
                    unsigned long objecthash, roomhash;
                    int sscr = sscanf(szname, "%8x_%8x", &roomhash, &objecthash);
                    assert(sscr >= 2);
                        
                    node = (NL_OBJ*)find_node((::RMX*)RMXBuffer, objecthash, roomhash);
                    assert(!!node);
                }

                struct rots rots = extractEulerAngles((double(*)[4])matrix.values);

                SUComponentDefinitionRef def = { SU_INVALID };
                b(SUComponentInstanceGetDefinition, szinstances[instances], &def);

                SUStringRelease(&name);

                b(SUStringCreate, &name);

                b(SUComponentDefinitionGetName, def, &name);

                b(SUStringGetUTF8, name, sizeof szname, szname, std::array<size_t, 1> { 0 }.data());

                size_t objid;
                int sscr = sscanf(szname, "Obj%zu", &objid);
                assert(sscr >= 1);

                SUStringRelease(&name);

                node->MeshObjIndex = objid;

                /*
                if (node->ObjectRotation.x || node->ObjectRotation.y || node->ObjectRotation.z) {
                    printf("extreacted rot %f, %f, %f\n", rots.x, rots.y, rots.z);
                    printf("real rot %f, %f, %f\n", node->ObjectRotation.x, node->ObjectRotation.y, node->ObjectRotation.z);
                }

                printf("trans: extreacted %f, %f, %f \n", matrix.values[3 * 4 + 0], matrix.values[3 * 4 + 1], matrix.values[3 * 4 + 2]);
                printf("trans: real %f, %f, %f - %d\n", node->ObjectTransform.x, node->ObjectTransform.y, node->ObjectTransform.z);*/

                node->ObjectTransform.x = matrix.values[3 * 4 + 0];
                node->ObjectTransform.y = matrix.values[3 * 4 + 1];
                node->ObjectTransform.z = matrix.values[3 * 4 + 2];

                node->ObjectRotation.x = rots.x;
                node->ObjectRotation.y = rots.y;
                node->ObjectRotation.z = rots.z;
            }

            fwrite((unsigned char*)RMXBuffer_, szRMXFile, 1, fopen("out.RMX", "wb"));

            SUTerminate();
            system("PAUSE");

            return 0;
        }

        // Create an empty model
        SUModelRef model = SU_INVALID;
        b(SUModelCreate, &model);
        SUEntitiesRef entities;
        b(SUModelGetEntities, model, &entities);
        char name[0x20];
        
        SUComponentDefinitionRef(*Definitions) = (decltype(Definitions))malloc(ZoneBuffer->ObjsPtr->AmountObjInfoPtrs * sizeof * Definitions);
        SUComponentDefinitionRef(*DefinitionsZone) = (decltype(Definitions))malloc(MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects * sizeof * Definitions);
        SUComponentDefinitionRef(*DefinitionsRooms) = (decltype(Definitions))malloc(ZoneBuffer->MshPtr->AmountRooms * sizeof * Definitions);
        SUMaterialRef* material = (decltype(material))malloc(ZoneBuffer->TextPtr->AmountTextInfo * sizeof(SUMaterialRef));
            
        fillMaterial(material, (::Zone*)ZoneBuffer);

        for (size_t(i) = 0; i < ZoneBuffer->ObjsPtr->AmountObjInfoPtrs; ++i)
            SUSetInvalid(i[Definitions]),
            b(SUComponentDefinitionCreate, Definitions + i),
            sprintf(name, "Obj%zu", i),
            b(SUComponentDefinitionSetName, i[Definitions], name);

        for (size_t(i) = 0; i < MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects; ++i)
            SUSetInvalid(i[DefinitionsZone]),
            b(SUComponentDefinitionCreate, DefinitionsZone + i),
            sprintf(name, "MeshObj%zu", i),
            b(SUComponentDefinitionSetName, i[DefinitionsZone], name);
        
        currentMesh = &ZoneBuffer->MshPtr->MeshRooms.Room;
        auto* currroomz = &ZoneBuffer->MshPtr->MeshRooms;
        

        for (size_t(i) = 0; i < ZoneBuffer->MshPtr->AmountRooms; ++i)
            SUSetInvalid(i[DefinitionsRooms]),
            b(SUComponentDefinitionCreate, DefinitionsRooms + i),
            sprintf(name, "Room%zu", currroomz->RoomHash),
            b(SUComponentDefinitionSetName, i[DefinitionsRooms], name),
            currentMesh = (Mesh*)currentMesh->next,
            currroomz = (RoomMesh*)currentMesh,
            currentMesh = &currroomz->Room;

        b(SUModelAddComponentDefinitions, model, ZoneBuffer->ObjsPtr->AmountObjInfoPtrs, Definitions);
        b(SUModelAddComponentDefinitions, model, MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects, DefinitionsZone);
        b(SUModelAddComponentDefinitions, model, ZoneBuffer->MshPtr->AmountRooms, DefinitionsRooms);
        currentMesh = &ZoneBuffer->MshPtr->MeshRooms.Room;
        currroomz = &ZoneBuffer->MshPtr->MeshRooms;
       // std::vector<::Mesh*> mshs;
        //mshs.reserve(ZoneBuffer->MshPtr->AmountRooms);
        SUEntitiesRef defentities = SU_INVALID;
        for (size_t(i) = 0; i < ZoneBuffer->MshPtr->AmountRooms; ++i)
            //mshs.push_back((::Mesh*)currentMesh),
            //SUEntitiesRef defentities = SU_INVALID;
            SUSetInvalid(defentities),
            b(SUComponentDefinitionGetEntities, i[DefinitionsRooms], &defentities),
            fillMesh(defentities, (::Mesh*)currentMesh, material),
            currentMesh = (Mesh*)currentMesh->next,
            currroomz = (RoomMesh*)currentMesh,
            currentMesh = &currroomz->Room;

        //concurrency::parallel_for(dword(0), ZoneBuffer->MshPtr->AmountRooms, [&](dword i) {
        //    SUEntitiesRef defentities = SU_INVALID;
        //    SUSetInvalid(defentities),
        //    b(SUComponentDefinitionGetEntities, i[DefinitionsRooms], &defentities),
         //   fillMesh(defentities, (::Mesh*)mshs[i], material);
        //});

        fillDefinitions(entities, Definitions, DefinitionsZone, (::Zone*)ZoneBuffer, material);
        //b(SUModelAddComponentDefinitions,model,MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects,Definitions);

        

        
        
        for (size_t RoomIndex = 0; currentRoom = RMXBuffer->Rooms[RoomIndex], RoomIndex < RMXBuffer->AmountRooms; ++RoomIndex) {
            struct SUTransformation matrix, matrixtransform, matrixrotation, * outmatrix;
            struct SUVector3D transform = { currentRoom->RoomTransform.x, currentRoom->RoomTransform.y, currentRoom->RoomTransform.z };
            struct SUPoint3D rotation = { 0 };
            SUComponentInstanceRef instance = { SU_INVALID };
            signed int roomidx = findroomindexbyhash((::Zone*)ZoneBuffer, currentRoom->RoomHash);
            if (roomidx != -1) {
                b(SUComponentDefinitionCreateInstance, DefinitionsRooms[roomidx], &instance);
                outmatrix = get_transform(&matrix, &transform, &rotation);
                if (outmatrix)
                    b(SUComponentInstanceSetTransform, instance, &matrix);
                char name[0x20];

                sprintf(name, "Room%d", RoomIndex);///*, 0[(*Code)[ObjHead(ZoneBuffer->ObjsPtr)].unknown2c]++*/);
                //printf("%x", objecthash);
                b(SUComponentInstanceSetName, instance, name);
                b(SUEntitiesAddInstance, entities, instance, NULL);
                //// b(SUComponentInstanceRelease, &instance);
            }
            for (struct NL_OBJ* (currentObject) = currentRoom->FirstObject; currentObject != (struct NL_OBJ*)currentRoom; currentObject = currentObject->Next) {
                SUComponentInstanceRef instance = { SU_INVALID };
                //if (!(currentObject->MeshObjIndex < ZoneBuffer->ObjsPtr->AmountObjInfoPtrs))
                //    continue;
                b(SUComponentDefinitionCreateInstance, Definitions[currentObject->MeshObjIndex], &instance);
                struct SUVector3D transform = { currentObject->ObjectTransform.x, currentObject->ObjectTransform.y, currentObject->ObjectTransform.z };
                struct SUPoint3D rotation = { currentObject->ObjectRotation.x, currentObject->ObjectRotation.y, currentObject->ObjectRotation.z };

                outmatrix = get_transform(&matrix, &transform, &rotation);
                if (outmatrix)
                    b(SUComponentInstanceSetTransform, instance, &matrix);
                char name[0x20];

                sprintf(name, "%x_%x", currentRoom->RoomHash, currentObject->ObjectHash);///*, 0[(*Code)[ObjHead(ZoneBuffer->ObjsPtr)].unknown2c]++*/);
                //printf("%x", objecthash);
                b(SUComponentInstanceSetName, instance, name);
                b(SUEntitiesAddInstance, entities, instance, NULL);
                //b(SUComponentInstanceRelease, &instance);
            }
        }
        size_t count = 0;
        //b(SUModelPurgeUnusedLayers, model, &count);
        printf("purged %zu\n", count);
        //b(SUModelAddComponentDefinitions,model,MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects,Definitions);
        SUModelSaveToFileWithVersion(model, "new_model.skp", SUModelVersion_Current),
            //SUModelRelease(&model),
            SUTerminate(),
            system("PAUSE");
    }
}

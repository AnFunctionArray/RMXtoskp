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
#include <assert.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define a(function,...)(function(__VA_ARGS__))
#define b(function,...)(assert(function(__VA_ARGS__)==SU_ERROR_NONE))
size_t
initFile(file,fileBuffer,filename,access)
    FILE(**file);
    char(**fileBuffer),*filename,*access;
{
    *file=a(fopen,(filename),access);
    a(fseek,*file,0,SEEK_END);
    size_t(fileSize)=a(ftell,*file);
    a(fseek,*file,0,SEEK_SET),
    *fileBuffer=a(malloc,fileSize),
    a(fread,*fileBuffer,sizeof(char),fileSize,*file);
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

    return (struct rots){ pitch * (180 / M_PI), roll * (180 / M_PI), yaw * (180 / M_PI) };
}

((main)(number,arguments)) //grand entry
    int number;
    char(**arguments);
{ //ugly braces
    //read the file maybe
		bool wearewriting = false;
    FILE(*RMXFile),*ZoneFile;
    struct RMX(*RMXBuffer);
    struct Room(*currentRoom);
    struct Zone(*ZoneBuffer);
    size_t szRMXFile;
    szRMXFile = initFile(&RMXFile, &RMXBuffer, 1[arguments], "r+b");
    initFile(&ZoneFile,&ZoneBuffer,2[arguments],"r+b");

    struct Mesh(*currentMesh);
    //It is not uncommon to include a header file mid function definition
    //It is also equally common to specify address base
#define rmxpointer __based(RMXBuffer)
#define roompointer __based(currentRoom)
#include "GameFormatRMX.h"
#define meshpointer __based(currentMesh)
#define zonepointer __based(ZoneBuffer)
#include "GameFormatZone.h"

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

            struct FloatingPosition pos = { .x = matrix.values[12], .z = matrix.values[14], .y = matrix.values[13], .d = matrix.values[15] };

            SUStringRef name = SU_INVALID; char szname[100];
            char sznametoinsert[100];

            b(SUStringCreate, &name);

            b(SUComponentInstanceGetName, szinstances[instances], &name);

            b(SUStringGetUTF8, name, sizeof szname, szname, (size_t[]) { 0 });
#define rmxpointer __based(RMXBuffer)
#define roompointer
#include "GameFormatRMX.h"
            struct NL_OBJ* node;

            if (strchr(szname, ':')) {
                unsigned long objecttoinherithash, roomtoniheritobjfromhash;
                unsigned long roomhashtarget;
                void* addNode(psizeRMXbuffer, pRMXBuffer, basedOnNode, basedOnRoom, inRoom);
                int nread = sscanf(szname, "%8x_%8x:%8x_%s", &roomtoniheritobjfromhash, &objecttoinherithash, &roomhashtarget, sznametoinsert);
                assert(nread >= 4);
                node = addNode(&szRMXFile, &RMXBuffer, objecttoinherithash, roomtoniheritobjfromhash, roomhashtarget);
                node->ObjectHash = djb_hash(sznametoinsert);
            }
            else {
                unsigned long objecthash, roomhash;
                assert(sscanf(szname, "%8x_%8x", &roomhash, &objecthash) >= 2);
                void* find_node(RMXBuffer, node, room);
                node = find_node(RMXBuffer, objecthash, roomhash);
                assert(node);
            }

            struct rots rots = extractEulerAngles(matrix.values);

            SUComponentDefinitionRef def = { SU_INVALID };
            b(SUComponentInstanceGetDefinition, szinstances[instances], &def);

            SUStringRelease(&name);

            b(SUStringCreate, &name);

            b(SUComponentDefinitionGetName, def, &name);

            b(SUStringGetUTF8, name, sizeof szname, szname, (size_t[]) { 0 });

            size_t objid;

            assert(sscanf(szname, "Obj%zu", &objid) >= 1);

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

        fwrite((unsigned char*)RMXBuffer, szRMXFile, 1, fopen("out.RMX", "wb"));

		SUTerminate();
		system("PAUSE");

		return;
	}

    // Create an empty model
    SUModelRef model=SU_INVALID;
    b(SUModelCreate,&model);
    SUEntitiesRef entities;
    b(SUModelGetEntities,model,&entities);
    char name[0x20];
    SUComponentDefinitionRef(*Definitions)=malloc(ZoneBuffer->ObjsPtr->AmountObjInfoPtrs *sizeof *Definitions);
    SUComponentDefinitionRef(*DefinitionsZone) = malloc(MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects * sizeof * Definitions);
    for (size_t(i) = 0; i < ZoneBuffer->ObjsPtr->AmountObjInfoPtrs; ++i)
        i[Definitions] = (SUComponentDefinitionRef){ SU_INVALID },
        b(SUComponentDefinitionCreate, Definitions + i),
        sprintf(name, "Obj%zu", i),
        b(SUComponentDefinitionSetName, i[Definitions], name);

    for (size_t(i) = 0; i < MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects; ++i)
        i[DefinitionsZone] = (SUComponentDefinitionRef){ SU_INVALID },
        b(SUComponentDefinitionCreate, DefinitionsZone + i),
        sprintf(name, "MeshObj%zu", i),
        b(SUComponentDefinitionSetName, i[DefinitionsZone], name);

	b(SUModelAddComponentDefinitions, model, ZoneBuffer->ObjsPtr->AmountObjInfoPtrs, Definitions);
    b(SUModelAddComponentDefinitions, model, MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects, DefinitionsZone);
    fillDefinitions(entities,Definitions, DefinitionsZone,ZoneBuffer);
    //b(SUModelAddComponentDefinitions,model,MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects,Definitions);

    for(size_t RoomIndex=0;currentRoom=RMXBuffer->Rooms[RoomIndex],RoomIndex<RMXBuffer->AmountRooms;++RoomIndex)
        for (struct NL_OBJ* (currentObject) = currentRoom->FirstObject; currentObject != currentRoom; currentObject = currentObject->Next) {
            SUComponentInstanceRef instance = { SU_INVALID };
            b(SUComponentDefinitionCreateInstance, Definitions[currentObject->MeshObjIndex], &instance);
            struct SUTransformation matrix, matrixtransform, matrixrotation, *outmatrix;
            struct SUVector3D transform = { currentObject->ObjectTransform.x, currentObject->ObjectTransform.y, currentObject->ObjectTransform.z };
            struct SUPoint3D rotation = { currentObject->ObjectRotation.x, currentObject->ObjectRotation.y, currentObject->ObjectRotation.z };

            struct SUTransformation* get_transform(matrix, rotation, transformation);
            outmatrix = get_transform(&matrix, &rotation, &transform);
            if (outmatrix)
                b(SUComponentInstanceSetTransform, instance, &matrix);
            char name[0x20];

            sprintf(name, "%x_%x", currentRoom->RoomHash, currentObject->ObjectHash);///*, 0[(*Code)[ObjHead(ZoneBuffer->ObjsPtr)].unknown2c]++*/);
                //printf("%x", objecthash);
            b(SUComponentInstanceSetName, instance, name);
            b(SUEntitiesAddInstance, entities, instance, NULL);
        }
    //b(SUModelAddComponentDefinitions,model,MeshesObjects(ZoneBuffer->MshPtr)->AmountObjects,Definitions);
    SUModelSaveToFile(model, "new_model.skp"),
    //SUModelRelease(&model),
    SUTerminate(),
    system("PAUSE");
}

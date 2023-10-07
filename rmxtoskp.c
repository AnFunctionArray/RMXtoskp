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
#include <math.h>
#define a(function,...)(function(__VA_ARGS__))
#define b(function,...)(assert(function(__VA_ARGS__)==SU_ERROR_NONE))
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
    initFile(&RMXFile,&RMXBuffer,1[arguments],"r+b"),
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

			SUStringRef name = SU_INVALID; char szname[260];

			b(SUStringCreate, &name);

			b(SUComponentInstanceGetName, szinstances[instances], &name);

			b(SUStringGetUTF8, name, sizeof szname, szname, (size_t[]) { 0 });

			unsigned long objectindex;

			sscanf(szname, "%u_", &objectindex);

			printf("%f, %f, %f - %d\n", pos.x, pos.y, pos.z, objectindex);
		}

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

#include <stdio.h>
#include "List.h"

static char *const DefaultMapFilepath = "./FRANCE.MAP";

/** Program input parameter count */
enum ArgsParamsCount {
    ArgsParamCount_NoInput = 1,
    ArgsParamCount_OnlyStartName = 2,
    ArgsParamCount_StartAndGoalName = 3,
    ArgsParamCount_ALL = 4,
};

/** Possible input parameters given to the program*/
enum ArgsInputParams {
    ArgsInputParam_StartCity = 1,
    ArgsInputParam_GoalCity = 2,
    ArgsInputParam_MapPath = 3
};

int main12(int argc, char** args) {

    char *startCity = 0;
    char *destCity = 0;
    char *mapFilePath = DefaultMapFilepath;

    // Check program input parameters
    switch(argc){
        case ArgsParamCount_NoInput: {
            startCity = (char*)malloc(MAX_CITYNAME_LENGTH);
            destCity = (char*)malloc(MAX_CITYNAME_LENGTH);

            // No parameters given
            printf("What is the starting city?\n");
            scanf("%s", startCity);
            printf("What is the destination city?\n");
            scanf("%s", destCity);
            break;
        }
        case ArgsParamCount_OnlyStartName: {
            // Goal city not specified as input param, ask for it.
            destCity = (char*)malloc(MAX_CITYNAME_LENGTH);
            printf("Which city do you want to go?\n");
            scanf("%s", destCity);
            startCity = args[ArgsInputParam_StartCity];
            break;
        }
        case ArgsParamCount_StartAndGoalName: {
            startCity = args[ArgsInputParam_StartCity];
            destCity = args[ArgsInputParam_GoalCity];
            break;
        }
        case ArgsParamCount_ALL: {
            startCity = args[ArgsInputParam_StartCity];
            destCity = args[ArgsInputParam_GoalCity];
            mapFilePath = args[ArgsInputParam_MapPath];
            break;
        }
        default: {
            printf("Incorrect input.\nInput commands: startCity [destCity] [filepathMap, Default=\'./FRANCE.MAP\']\n");
            return 0;
        }
    }

    List *pCityList = 0;
    status ret = createMap(mapFilePath, &pCityList);
    if(ret != OK) {
        printf("While populating map from %s\nError: %s\n", mapFilePath, message(ret));
        return(0-ret);
    }

    // Start finding Route
    printf("\nFinding shortest route\nFrom:\t%s\nTo:\t%s\n\n", startCity, destCity);
    ret = findPath(pCityList,startCity, destCity);
    if(ret != OK) {
        printf("Error: %s.\n", message(ret));
        return(0-ret);
    }

    // Clean up
    destroyMap(pCityList);
    if(argc == ArgsParamCount_NoInput) {
        free(startCity);
        free(destCity);
    }
    else if(argc == ArgsParamCount_OnlyStartName){
        free(destCity);
    }

    return 0;
}

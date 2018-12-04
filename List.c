#include <stdio.h>
#include "List.h"
#include <math.h>
#include <string.h>
#include <limits.h>

static Node* available = 0;

List* newList(compFun comp,prFun pr) {
	List* list = malloc(sizeof(List));
	if (list == NULL) {
		return 0;
    }
        else {
	list->nelts = 0;
	list->head = NULL;
	list->comp = comp;
	list->pr = pr;
	return list;
        }
}


void delList(List* l) {
	Node* n1 = l->head;
	while (n1 != NULL) {
		Node* n2 = n1->next;
		free(n1);
		n1 = n2;
	}
	free(l);
}

status nthInList(List* l,int n,void** e) {
	Node* nl = l->head;
	while (n--)	{
		if (nl == NULL) {
			break;
        }
		nl = nl->next;
	}
	if (nl == NULL) {
		return ERRINDEX;
	}
	*e = nl->val;
	return OK;
}


status addListAt (List* l, int pos, void* elt) {
    if (pos <= 0 || pos > l->nelts+1)
        return ERRINDEX;
    else {

        /* get a new Node and increment length */
        Node * toAdd = available;
        if (!toAdd) toAdd = (Node*) malloc(sizeof(Node));
        else available = available->next;
        if (!toAdd) return ERRALLOC;
        l->nelts++;
        toAdd->val = elt;

        /* if pos is 1, must change list head */
        if (pos == 1) {
            toAdd->next = l->head;
            l->head = toAdd;
        }

        /* otherwise we need a temporary pointer */
        else {
            Node * tmp = l->head;

            /* tmp points to the predecessor */
            while (pos-- > 2) tmp = tmp->next;

            /* actual insertion */
            toAdd->next = tmp->next;
            tmp->next = toAdd;
        }
        return OK;
    }
}



status remFromListAt	(List* l,int pos, void** res) {
    Node *toRem = l->head;

    if (pos <= 0 || pos > l->nelts) return ERRINDEX;

    /* particular case: must remove list head */

    if (pos == 1)
        l->head = toRem->next;

    /* otherwise point to predecessor to change links */
    else {
        Node * prec = toRem;
        while (pos-- > 2) prec = prec->next;
        toRem = prec->next;
        prec->next = toRem->next;
    }

    *res = toRem->val;
    toRem->next = available;
    available = toRem;
    l->nelts--;
    return OK;
}


status remFromList(List* l,void* e) {
	Node* nl = l->head;
	if (nl == NULL) {
		return ERRABSENT;
    }
	if (nl->val == e) {
		l->head = nl->next;
	} else {
		Node* n2 = nl;
		nl = nl->next;
		while (nl != NULL) {
			if (nl->val == e) {
				n2->next = nl->next;
				break;
			}
			n2 = nl;
			nl = nl->next;
		}
		if (nl == NULL) {
			return ERRABSENT;
		}
	}
	free(nl);
	l->nelts--;
	return OK;
}


status displayList(List* l) {
	printf("[ ");
	Node* nl = l->head;
	while (nl != NULL) {
		(*l->pr)(nl->val);
		putchar(' ');
		nl = nl->next;
	}
	printf("]");
}


void forEach(List* l,void(*f)(void*)) {
	Node* nl = l->head;
	while (nl != NULL) {
		(*f)(nl->val);
		nl = nl->next;
	}
}
/*return the number of elements in given list */
int lengthList(List* l) {
	return l->nelts;
}



static Node* previous(List* l, void* e) {
    Node * prec = l->head;
    if (l->nelts == 0) return 0;
    if (!(l->comp)(e,prec->val)) return (Node*)1;

    /* prec must address element prior to the one to remove */
    while (prec->next && (l->comp)(e,prec->next->val))
        prec = prec->next;

    /* here,
     *  - either we address nothing (no such element)
     *  - or we address the element prior to the one we're looking for
     */
    return prec;
}

/* add given element to given list according to compFun function */
status addList(List* l,void* e) {
	Node* n1 = malloc(sizeof(Node));
	if (n1 == NULL)
		return ERRALLOC;
	n1->val = e;
	Node* n2 = l->head;
	if (n2 == NULL) {
		l->head = n1;
		n1->next = NULL;
	} else if ((*l->comp)(e, n2->val) <= 0) {
		n1->next = l->head;
		l->head = n1;
	} else {
		Node* prev = n2;
		n2 = n2->next;
		while (n2 != NULL) {
			if ((*l->comp)(e, n2->val) <= 0) {
				break;
			}
			prev = n2;
			n2 = n2->next;
		}
		prev->next = n1;
		n1->next = n2;
	}
	l->nelts++;
	return OK;
}
/* check for the list contain given element */
Node* isInList(List* l,void* e) {
	Node* nl = l->head;
	while (nl != NULL) {
		if (e == nl->val) {
			return (Node*)1;
        }
		nl = nl->next;
	}
	return 0;
}



int DistOfTwoCity(void * citA, void * citB){


    int cp = ((City *)citA)->Startdistance + (~((City *)citA)->Startdistance + 1);
    return cp;
}


int citycmp(void * ct1, void * ct2){
    City * c1 = (City * )ct1;
    City * c2=  (City * )ct2;
return strcasecmp( c1->name, c2->name );
}

int startFinalDistance(City * ct,City * finish){

    float distance;
    distance = sqrt((ct->latitude - finish->latitude) * (ct->latitude - finish->latitude) + (ct->longitude - finish->longitude) * (ct->longitude - finish->longitude));
    return distance;
}

int Neighbcmp(void * nbA, void * nbB){
    int cp =  strcmp(((Neighbours * ) nbA) -> city->name, (((Neighbours * ) nbB) -> city->name));
    return cp;
}
void NeigDisp(void * disp){
    Neighbours * nbs = disp;
    printf("%s%d\n",nbs->city->name,nbs->distance);
}

void CitDisp(void * disp){
    City * dp = disp;
    printf("\n %s  %d  %d  %d  \t", dp->name,dp->latitude,dp->longitude,dp->neighbours->nelts);
    forEach(dp->neighbours, NeigDisp);
}



City * BuildCity(char* cty, int lat, int lgt){
    City * ct = malloc(sizeof(City));
    ct->name = ( char* )malloc( MAX_LENGTH  );
    strncpy( ct->name , cty, MAX_LENGTH );
    ct->latitude = lat;
    ct->longitude = lgt;
    ct->Startdistance = -HUGE_VAL;
    ct->neighbours = newList(Neighbcmp, CitDisp);
    ct->neighbours->comp=Neighbcmp;
    return ct;
}
City * createTheCity() {
    City * city = (City * ) malloc(sizeof(City));
    city -> name = (char * ) malloc(MAX_LENGTH);

    city -> latitude = 0;
    city -> longitude = 0;
    city -> neighbours = 0;
    city -> ctr = 0;
    city -> Startdistance = 0;
    city -> Finishdistance = 0;

    return city;
}

int Eltcmp (void * stA, void * stB) {

    if (!stA && !stB) return 0;
    if (stA && !stB) return 1;
    if (!stA && stB) return -1;

    while (stA && stB && stA == stB) stA++, stB++;

    if (!stA && !stB) return 0;
    else if (stA > stB) return 1;
    else return -1;
}

void CityDisplay(void * disp){
    City * dp = ( City* )disp;
    printf("\n %s  %d  %d  %d  \t", dp->name,dp->latitude,dp->longitude,dp->neighbours->nelts);
    forEach(dp->neighbours, NeigDisp);
}



City* findCity( List* cityList, char* name )
{
    Node* currentNode = cityList->head;
    while( currentNode )
    {
        City* crntCity = ( City* )currentNode->val;
        if( strcasecmp( crntCity->name, name ) == 0)
        {
                printf("\n %s %d  %d \t", crntCity->name,crntCity->latitude,crntCity->longitude);
            return crntCity;

        }
        currentNode = currentNode->next;
    }
    return 0;
}

Neighbours * CreateNeighbour(City * city, int distance){

    Neighbours * nb = malloc(sizeof(Neighbours));
    nb->city = city;
    nb->distance = distance;
    return nb;
}


void NeighbourDisplay(void * disp){
    Neighbours * nbs = disp;
    printf("%s%d\n",nbs->city->name,nbs->distance);
}

void destroyNeighbour( Neighbours** neighbour )
{
    free( ( *neighbour)->city );
    free( *neighbour );
}

static int get_heuristic_cost( City* startCity, City* goalCity )
{
    return ( abs( startCity->latitude - goalCity->latitude ) + abs( startCity->longitude - goalCity->longitude ) ) / 4;
}

static City* getLowestfValueCity( List* openList )
{
    Node* currentNode = openList->head;
    if( !currentNode )
    {
        return 0;
    }
    City* highestFCity = ( City* )currentNode->val;
    int highestFvalue = highestFCity->Startdistance + highestFCity->Finishdistance;
    currentNode = currentNode->next;

    while( currentNode )
    {
        City* crntCity = ( City* )currentNode->val;
        if( highestFvalue > ( crntCity->Startdistance + crntCity->Finishdistance ) )
        {
            highestFCity = crntCity;
        }
        currentNode = currentNode->next;
    }
    return highestFCity ;
}

static void displaySearchResults( City* currCity )
{
    List* disList = newList( DistOfTwoCity, CityDisplay );
    while( currCity )
    {
        addListAt( disList,1, currCity );
        currCity = currCity->ctr;
    }
    displayList(disList);
    delList(disList);
}

// parse the France.Map file
List* parseMapFile( char*  filename )
{
    FILE* fileptr;
    //open file
    fileptr = fopen( filename, "r" );
    if( !fileptr )
    {
        return 0;
    }
    List* cityList = newList( citycmp,CityDisplay );
    char* cityName = ( char* )malloc(MAX_LENGTH);
    City* newCity = 0;
    // read line by line
    while ( !feof ( fileptr ) )
    {
		int latitude=-1;
        int longitude=-1;
        fscanf( fileptr, "%s %d %d", cityName, &latitude, &longitude );
        if( -1 != longitude  )
        {
            if( 0 != newCity )
            {
                addList( cityList, newCity );
            }
            //newCity = BuildCity(cityName,latitude,longitude);
			            newCity = createTheCity();
            newCity->neighbours = newList( Neighbcmp, NeighbourDisplay );
            strncpy( newCity->name , cityName, MAX_LENGTH );
            newCity->latitude = latitude;
            newCity->longitude = longitude;
        }
        else if( 0 != newCity )
        {
            Neighbours* neighbour = 0;
            neighbour = CreateNeighbour(cityName,latitude);
            strncpy( neighbour->city->name, cityName, MAX_LENGTH );
            neighbour->distance = latitude;
            addList( newCity->neighbours, neighbour );
        }
    }
    if( 0 != newCity )
    {
        addList( cityList, newCity );
    }
    fclose( fileptr );
    return cityList;
}



status findPath( List* cityList, char* srcCity, char* destCity )
{
    City* startCity = findCity( cityList, srcCity );
	    printf("Start City: %s\n", startCity -> name);
    City* goalCity = findCity( cityList, destCity );
	    printf("Start City: %s\n", goalCity -> name);
    if( !cityList || !startCity || !goalCity )
    {
        return ERRCITYNOTFOUND;
    }
    List* closedList = newList( citycmp, CityDisplay );

    List* openList = newList( citycmp, CityDisplay );
    // The cost of going from start to start is zero.
    startCity->Startdistance = 0;
    startCity->Finishdistance = get_heuristic_cost( startCity, goalCity );
    // directly adding start city to open list
    addList( openList, startCity );
    //loop all the cities in open list
    while( lengthList( openList ) > 0 )
    {
        City* currentCity = getLowestfValueCity( openList );
        remFromList( openList, currentCity );
        addList( closedList, currentCity );
        if( cityList->comp( currentCity , goalCity ) == 0  )
        {
            //we found the solution
            displaySearchResults( currentCity );
            break;
        }
        Node* neighbourNode = currentCity->neighbours->head;
        while( neighbourNode )
        {
            Neighbours* nextNeighbour = ( Neighbours* )neighbourNode->val;
            City* succCity = findCity( cityList, nextNeighbour->city->name );
            int sucNodeCost = currentCity->Startdistance + nextNeighbour->distance;
            City* nodeSucc = findCity( openList, succCity->name );
            if( nodeSucc!=0 )
            {
                if( sucNodeCost >= nodeSucc->Startdistance )
                {
                    neighbourNode = neighbourNode->next;
                    continue;
                }
                remFromList( openList, succCity );
            }
            nodeSucc = findCity( closedList, succCity->name );
            if( nodeSucc != 0 )
            {
                /// if successor city is on the CLOSED list but the existing
                //one is as good or better then discard this successor and continue
                if( sucNodeCost >= nodeSucc->Startdistance )
                {
                    neighbourNode = neighbourNode->next;
                    continue;
                }
                remFromList( closedList, succCity );
            }
            //Set the parent of successor city  to current city
            succCity->ctr = currentCity;
            // set g value of successor city  as new successor Node Cost
            succCity ->Startdistance = sucNodeCost;
            // Set h to be the estimated distance to goal city(Using the heuristic function)
            succCity->Finishdistance = get_heuristic_cost( succCity, goalCity );
            addList( openList, succCity );
            neighbourNode = neighbourNode->next;
        }

    }
    delList( openList );
    delList( closedList );
    return OK;
}

void 	destroyCities( List* listCities )
{
    Node* node  = listCities->head;
    while( node )
    {
        City* tempCity = node->val;
        Node* nodeN = tempCity->neighbours->head;
        while( nodeN )
        {
            Neighbours* tempNeigh = nodeN->val;
            destroyNeighbour( &tempNeigh );
            nodeN = nodeN->next;
        }
        destroyCity( &tempCity );
        node = node->next;
    }
    delList( listCities );
}

void destroyCity( City** city )
{
    free( ( *city )->name );
    delList( ( *city )->neighbours );
    free( *city );
}


static int compCitiesBasedOnName(void *s1, void *s2) {
    City* city1 = (City*)s1;
    City* city2 = (City*)s2;
    return strcmp(city1->name, city2->name);
}

static int compCitiesBasedOnFinishdistance(void *s1, void *s2) {
    City* city1 = (City*)s1;
    City* city2 = (City*)s2;
    return (city1->Finishdistance - city2->Finishdistance);
}


status getOrCreateCity(char *cityName, List *pToCityList, City **city) {
    // Search the list for the City node
    City *foundCity = findCity(pToCityList,cityName);
    if(!foundCity) {
        // Create the new city
        City *pNewCity = (City*)malloc(sizeof(City));
        if(!pNewCity) {
            return ERRALLOC;
        }
        // Create memory for name
        pNewCity->name = (char*)malloc(MAX_CITYNAME_LENGTH);
        if(!pNewCity->name) {
            free(pNewCity);
            return ERRALLOC;
        }

        // Copy the name
        strncpy(pNewCity->name, cityName, MAX_CITYNAME_LENGTH);

        // Add to cityList
        status ret;
        if((ret = addList(pToCityList, pNewCity )) != OK) {
            // Unable to add, free it or it will be lost.
            free(pNewCity);
            *city = 0;
            return ret;
        }
        *city = pNewCity;
    }
    else {
        *city = foundCity;
    }
    return OK;
}

status createMap(char *path, List **cityMapList) {
    FILE *file;
    char cityName[MAX_CITYNAME_LENGTH];
    int mapParam1;
    int mapParam2;

    // Create a new cities list
    *cityMapList = newList(compCitiesBasedOnName, compCitiesBasedOnFinishdistance);

    // Open the file
    file = fopen(path,"r");
    if(!file) {
        printf("Error while opening: %s\n", path);
        return ERROPEN;
    }

    // Read complete file and parse
    City *curCity = 0;
    while(!feof(file)) {
        int paramsCount = fscanf(file, "%s\t\t%d\t%d", cityName, &mapParam1, &mapParam2);
        switch (paramsCount){
            case 3:
            {
#ifdef ENABLE_DEBUG_INFO
                printf("%s %d %d\n",cityName, mapParam1, mapParam2);
#endif
                // Create the new city
                City *city = 0;
                status ret;
                if((ret = getOrCreateCity(cityName, *cityMapList, &city)) != OK) {
                    return ret;
                }
                // Initialise the city
                city->latitude = mapParam1;
                city->longitude = mapParam2;
                city->Startdistance = INT_MAX;
                city->Finishdistance = 0;
                city->neighbours = 0;
                city->ctr = 0;
                curCity = city;
                break;
            }
            case 2:
            {
#ifdef ENABLE_DEBUG_INFO
                printf("\t%s %d\n",cityName, mapParam1);
#endif
                // Check if the city is already available, if not add.
                City *city = 0;
                Neighbours *neighbour=0;
                status ret;
                if((ret = getOrCreateCity(cityName, *cityMapList, &city)) != OK) {
                    return ret;
                }
                if((neighbour = CreateNeighbour(city, mapParam1)) != 0) {
                    return OK;
                }
                break;
            }
        }
    }
    // Close the file
    fclose(file);

#ifdef ENABLE_DEBUG_INFO
    printf("Found cities: %d\n", lengthList(*cityMapList));
    displayList(*cityMapList);
#endif
    return OK;
}

void destroyMap(List *cityList){
    if(!cityList){
        return;
    }

    // Free all allocated cities
    while(cityList->head) {
        Node *pNodeTmp = cityList->head->next;
        free( ((City*)cityList->head->val)->name);     // Free allocated memory for the name
        free(cityList->head->val);                          // Free the City
        cityList->head = pNodeTmp;
    }

    // Free the list
    delList(cityList);
}


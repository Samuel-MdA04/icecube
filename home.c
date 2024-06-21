#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100
#define HASH_SIZE 1000

typedef struct {
	char city[N];
	float temp, med, min, max;
}STATIONS;

typedef struct node {
	STATIONS station;
	struct node *next;
}NODE;

NODE *hashTable[HASH_SIZE];

//calcula índice para a hash table
unsigned hash(char *str) {
	unsigned hash = 0;
	while(*str != '\0') {
		hash *= 32;
		hash += *str;
		str++;
	}
	return hash % HASH_SIZE;
}
//insere node na hash table
void insert(NODE *table[], STATIONS station) {
	unsigned i = hash(station.city);
	NODE *newNode = malloc(sizeof(NODE));
	if(newNode == NULL) {
		printf("Allocation error.\n");
		exit(1);
	}
	newNode->station = station;
	newNode->next = table[i];
	table[i] = newNode;
}
//procura por índices que se repetem na hash table
NODE * search(NODE *table[], char *city) {
	unsigned i = hash(city);
	NODE *current = table[i];
	while(current != NULL) {
		if(strcmp(current->station.city, city) == 0) {
			return current;
		}
		current = current->next;
	}
	return NULL;
}
//organiza os elementos dos arquivos na struct STATIONS
void arrange_str(char str[N], STATIONS *station) {
	int i = 0;
	while(str[i] != ';') {
		station->city[i] = str[i];
		i++;
	}
	station->city[i] = '\0';
	int sign;
	if(str[i+1] == '-') {
		sign = -1;
		i += 2;
	}
	else {
		sign = 1;
		i++;
	}
	if(str[i+1] != '.') {
		station->temp = (str[i] - 48) * 10;
		station->temp += (str[i+1] - 48);
		station->temp += (str[i+3] - 48) * 0.1;
		station->temp += (str[i+4] - 48) * 0.01;
	}
	else {
		station->temp = (str[i] - 48);
		station->temp += (str[i+3] - 48) * 0.1;
		station->temp += (str[i+4] - 48) * 0.01;
	}
	station->temp *= sign;
	station->min = 100.0; //placeholder
	station->max = -150.0; //placeholder
}
//função que compara duas estações (ordem alfabética)
int cmpfunc(const void *s1, const void *s2) {
	STATIONS *station1 = (STATIONS*)s1;
	STATIONS *station2 = (STATIONS*)s2;
	return strcmp(station1->city, station2->city);
}
//start main
int main() {
	char strTemp[N];
	int i, j = 0, count = 0;
	float sum, denom;
	//abertura dos arquivos para leitura + verificação de erros na abertura
	FILE *ftxt = fopen("data/measurements.txt","r");
	FILE *fcsv = fopen("data/weather_stations.csv","r");
	if(ftxt == NULL) {
		printf("Error in the reading of measurements.txt\n");
		exit(1);
	}
	if(fcsv == NULL) {
		printf("Error in the reading of weather_stations.csv\n");
		exit(1);
	}
	//inicializa a hash table
	for(i = 0; i < HASH_SIZE; i++) {
		hashTable[i] = NULL;
	}
	//insere os elementos na hash table
	while(fgets(strTemp, N, fcsv) != NULL) {
		STATIONS station;
		arrange_str(strTemp, &station);
		insert(hashTable, station);
		count++;
	}
	//faz comparações com os elementos de measurements.txt + calcula média e minmax
	while(fgets(strTemp, N, ftxt) != NULL) {
		STATIONS station;
		arrange_str(strTemp, &station);
		NODE *node = search(hashTable, station.city);
		if(node != NULL) {
			sum = node->station.temp;
			denom = 1;
			if(node->station.min > station.temp) {
				node->station.min = station.temp;
			}
			if(node->station.max < station.temp) {
				node->station.max = station.temp;
			}
			sum += station.temp;
			denom++;
			node->station.med = sum / denom;
		}
		else {
			insert(hashTable, station);
		}
	}
	//ordenação das estações por ordem alfabética
	STATIONS *all = malloc(count * sizeof(STATIONS));
	if(all == NULL) {
		printf("Allocation error.\n");
		exit(1);
	}
	for(i = 0; i < HASH_SIZE; i++) {
		NODE *node = hashTable[i];
		while(node != NULL) {
			all[j] = node->station;
			node = node->next;
			j++;
		}
	}
	qsort(all, count, sizeof(STATIONS), cmpfunc);
	//saída na stdout
	for(i = 0; i < count; i++) {
		printf("media de %s: %.2f C | ", all[i].city, all[i].med);
		printf("minima: %.2lf C maxima: %.2lf C\n", all[i].min, all[i].max);
	}
	//desalocação de memória dos nodes da hash table
	for(i = 0; i < HASH_SIZE; i++) {
		NODE *node = hashTable[i];
		while(node != NULL) {
			NODE *temp = node;
			node = node->next;
			free(temp);
		}
	}
	//fecha arquivos abertos e desaloca o array ordenado
	fclose(ftxt);
	fclose(fcsv);
	free(all);
	return 0;
}
//end main

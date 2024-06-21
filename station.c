#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100

typedef struct {
    char city[N];
    float temp;
}STATIONS;

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
}

int main() {
    char strTemp[N];
    STATIONS station;
    char cityName[N];
    FILE *ftxt = fopen("data/measurements.txt", "r");
    FILE *fcsv = fopen("data/weather_stations.csv", "r");
    if (ftxt == NULL) {
        printf("Error in the reading of measurements.txt\n");
        exit(1);
    }
    if (fcsv == NULL) {
        printf("Error in the reading of weather_stations.csv\n");
        exit(1);
    }
    gets(cityName);
    while (fgets(strTemp, N, fcsv) != NULL) {
        arrange_str(strTemp, &station);
        if (strcmp(station.city, cityName) == 0) {
            printf("%s %.2f\n",station.city, station.temp);
        }
    }
    while (fgets(strTemp, N, ftxt) != NULL) {
        arrange_str(strTemp, &station);
        if (strcmp(station.city, cityName) == 0) {
            printf("%s %.2f\n",station.city, station.temp);
        }
    }
    fclose(ftxt);
    fclose(fcsv);
    return 0;
}

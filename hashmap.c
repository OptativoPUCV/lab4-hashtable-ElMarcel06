#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if(map==NULL || key==NULL) return;
    if(map->size==map->capacity){
        enlarge(map);
    }
    else{
        long posicion = hash(key,map->capacity);
        while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL){
            if(is_equal(map->buckets[posicion]->key,key)){
                map->buckets[posicion]->value=value;
                return;
            }
          else{
            posicion=(posicion+1)%map->capacity;
            
          }
          
        }
        map->buckets[posicion]=createPair(key,value);
        map->size++;
        
    }
  
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** aux = map->buckets;
  map->capacity = map->capacity*2;
  map->buckets = (Pair **)calloc(map->capacity,sizeof(Pair *));
  map->size=0;
  for(int i=0;i<map->capacity/2;i++){
    if(aux[i]!=NULL){
      insertMap(map,aux[i]->key,aux[i]->value);
      
    }
    else{
      continue;
    }
    
  }
  
  free(aux);
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity, sizeof(Pair *));
  map->size = 0;
  map->capacity = capacity;
  map->current = -1;
  return map;
  
  
}

void eraseMap(HashMap * map,  char * key) {    
  if(map == NULL || key == NULL) return;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion]!=NULL){
    if(is_equal(map->buckets[posicion]->key,key)){
      map->buckets[posicion]->key=NULL;
      map->size--;
      
    }
    else{
      posicion=(posicion+1)%map->capacity;
      
      
    }
    
  }

}

Pair * searchMap(HashMap * map,  char * key) {   
  if(map==NULL || key==NULL) return NULL;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion]!=NULL && map->buckets[posicion]->key!=NULL){
    if(is_equal(map->buckets[posicion]->key,key)){
      map->current=posicion;
      return map->buckets[posicion];
      
    }
    else{
      posicion=(posicion+1)%map->capacity;
      
    }
    
  }
  
    return NULL;
}

Pair * firstMap(HashMap * map) {
  if(map==NULL) return NULL;
  for(int i = 0; i<map->capacity;i++){
    if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
      map->current=i;
      return map->buckets[i];
    }
  }
  
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if(map==NULL) return NULL;
  if(map->current==-1) return NULL;
  
  for(int i = map->current+1; i<map->capacity;i++){
    if(map->buckets[i]!=NULL && map->buckets[i]->key!=NULL){
      map->current=i;
      
      return map->buckets[i];
      
    }  
    
  }

  return NULL;
}

#include <igraph.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "graph6_utils.h" 

#define MAXSTRLEN 80 

int heur_indep_lb(igraph_t* g) {
  igraph_t h;
  igraph_copy(&h, g);
  int count = 0;

  igraph_vector_t vec;
  igraph_vit_t allv;
  igraph_integer_t current_mindeg_vx;

  while(igraph_vcount(&h) > 0) {
    count++;

    // find a minimum degree vertex
    int current_mindeg = INT_MAX;
    igraph_vector_init(&vec,1);
    igraph_vit_create(&h,igraph_vss_all(),&allv);
    while (!IGRAPH_VIT_END(allv)) { 
      igraph_degree(&h,&vec,igraph_vss_1(IGRAPH_VIT_GET(allv)),IGRAPH_ALL,1);
      if(current_mindeg > VECTOR(vec)[0]) {
        current_mindeg = VECTOR(vec)[0];
        current_mindeg_vx = IGRAPH_VIT_GET(allv);
      } 
      IGRAPH_VIT_NEXT(allv);
    } 
    igraph_vit_destroy(&allv); 

    // delete current_mindeg_vx and all its neighbours
    igraph_neighbors(&h,&vec,current_mindeg_vx,IGRAPH_ALL);
    igraph_vector_push_back(&vec,current_mindeg_vx);
    igraph_delete_vertices(&h, igraph_vss_vector(&vec));

    igraph_vector_destroy(&vec);
  }

  igraph_destroy(&h);
  return(count);
}

static inline int tau(igraph_t* g) {
  int alpha;
  igraph_independence_number(g, &alpha);
  return(igraph_vcount(g) - alpha);
}

int is_tau_critical(igraph_t* g) {
  int tau_g = tau(g);
  if(tau_g >= 7)
    return(0);
  igraph_t h;

  igraph_eit_t alle;
  igraph_eit_create(g,igraph_ess_all(IGRAPH_EDGEORDER_ID),&alle);

  while(!IGRAPH_EIT_END(alle)) {
    igraph_copy(&h, g);
    igraph_delete_edges(&h, igraph_ess_1(IGRAPH_EIT_GET(alle)));
    if(tau(&h) >= tau_g) {
      igraph_eit_destroy(&alle);
      igraph_destroy(&h);
      return(0);
    }
    igraph_destroy(&h);

    IGRAPH_EIT_NEXT(alle);
  }

  igraph_eit_destroy(&alle);
  return(1);
}

int main(int argc, char* argv[]) { 
  char line[MAXSTRLEN+2];
  int i;

  igraph_t g;
  while(scanf("%s\n", line) != EOF) {
    i = strlen(line);
    line[i] = '\n'; line[i+1] = '\0';
    read_graph6(&g, line);
    if(is_tau_critical(&g)) {
      printf("%s", line);
    }

    igraph_destroy(&g);
  }

  return(0);
}

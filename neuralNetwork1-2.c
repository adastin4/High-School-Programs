#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct network{
    size_t numOfLayers;
    size_t *layerSizes;
    double ***weights;
} Network;

double logistic(double x);
double **matmul(double **a, double **b, size_t aRow, size_t ab, size_t bCol);
double *vectorbyMatrix(double *a, double **b, size_t av, size_t bcol);
double vectorMult(double* x, double* y, size_t z);

void printMatrix(double **m, size_t rows, size_t cols);
void printVector(double *vm, size_t l);

double **createMatrix(size_t rows, size_t cols);
Network *createNetwork(size_t numOfLayers, size_t *layerSizes);
void destroyNetwork(Network *network);

double *feedForward(Network *network, double *input);
double backPropagate(Network *network, double *input, double *expected, double learningRate);
double *softmax(double *a, size_t length);

int main(void){
    size_t tsize = 400;
    size_t tesize = 50;
    size_t learningRate = 0.4;    
    int accuracy = 0;

    FILE* ftraining = fopen("mnist_train.csv", "r");
    
    if(ftraining == NULL){
        perror("Cannot open file \n");
        exit(EXIT_FAILURE);
    }
    
    double* expectedTrain = malloc(tsize*sizeof(double));
    double** trained = malloc(tsize*sizeof(double*));
    
    for(size_t i=0;i<tsize;++i)
      trained[i] = malloc(732*sizeof(double));
    
    char buffer[5000];
    
    for(size_t i = 0; i < tsize; ++i){
      fgets(buffer, sizeof(buffer), ftraining);
      char* tok = strtok(buffer, ",");
      expectedTrain[i] = atoi(tok);
      size_t j=0;
      for(;;){ 
          tok = strtok(NULL, ",");
          if(tok == NULL)
              break;
          trained[i][j++] = atoi(tok);
      }
    }

    FILE* ftesting = fopen("mnist_test.csv", "r");
    
    if(ftraining == NULL){
        perror("Cannot open file \n");
        exit(EXIT_FAILURE);
    }

    double* expectedTest = malloc(tesize*sizeof(double));
    double** tested = malloc(tesize*sizeof(double*));
    
    for(size_t i=0;i<tesize;++i)
      tested[i] = malloc(732*sizeof(double));
   
   char buffer2[5000];
    
    for(size_t i = 0; i<tesize; ++i){
      fgets(buffer2, sizeof(buffer2), ftesting);
      char* tok = strtok(buffer2, ",");
      expectedTest[i] = atoi(tok);
      size_t j=0;
      
      for(;;){ 
          tok = strtok(NULL, ",");
          if(tok == NULL)
              break;
          tested[i][j++] = atoi(tok);
      }
    }

    size_t* layerSizes = malloc(3*sizeof(size_t));

    int d = 512;
    layerSizes[0] = 732;
    layerSizes[1] = d; //single hidden layer
    layerSizes[2] = 10;  

    printf("Hidden layer size: %d\n", d); 

    Network* netwrk = createNetwork(3, layerSizes);
    
    int guess = 0;

    for(size_t i = 0; i < 10; ++i){
        if((int)expectedTest[i] == guess){
            ++accuracy;
        }
    }
    
    accuracy = accuracy/tesize;

    printf("Accuracy Before Training: %d \n", accuracy);
    //Training 

    accuracy = 0;

    for(size_t i = 0; i < tsize; ++i){
        backPropagate(netwrk, trained[i], expectedTrain, learningRate);
    }

    for(size_t i = 0;i < tesize;++i){
      double* r = feedForward(netwrk, tested[i]);
      guess = 0;
     
     puts("Values yielded from feedfoward (going over entire network): ");
      for(size_t j=0;j<10;++j)
          printf("%f ", r[j]);
      
      double max = -1;

      for(size_t j=0;j<10;++j)
          if(r[j] <= max){} 
          else{
              max = r[j];
              guess = j;
          }
      printf("\nReal value (from mnist): %d\nPredicted Value (using neural network): %d\n", (int)expectedTest[i], guess);
    }
     
    for(size_t i = 0; i < 10; ++i){
        if((int)expectedTest[i] == guess){
            ++accuracy;
        }
    }

    accuracy = accuracy/tesize;

    printf("Accuracy after Training: %d\n", accuracy);

    destroyNetwork(netwrk);
    
    return 0;
}

double logistic(double x){
    //return 1/(1+exp(-10*x))
    return 1.0/(1.0+exp(-(x-0.5)));
}

double **matmul(double **a, double **b, size_t aRow, size_t ab, size_t bCol){
    size_t i, j, k;
    double **out = malloc(aRow * sizeof(double*));
    for(i = 0; i<aRow;++i){
        out[i] = malloc(bCol*sizeof(double));
    }
    for(i = 0; i < aRow; ++i){
        for(j = 0; j<bCol; ++j){
            for(k = 0; k<ab; ++k){
                out[i][j] += a[i][k]*b[k][j];
            }
        }
    }
    return out;
}

double *vectorbyMatrix(double *a, double **b, size_t av, size_t bcol){
    return *matmul(&a,b,1,av,bcol);
}

double vectorMult(double* x, double* y, size_t z){
    double output = 0;
    for(size_t i=0;i<z;++i)
        output += x[i]*y[i];
      return output;
}

void printMatrix(double **m, size_t rows, size_t cols){
    for(size_t i = 0; i < rows; ++i){
        for(size_t j = 0; j< cols; ++j){
            printf("%f ",m[i][j]);
        }
        puts("");
    }
}

double** createMatrix(size_t rows, size_t cols){
    double** output = malloc(sizeof(double*)*rows);
    size_t i, j;
    for(i = 0;i < rows;i++)
        output[i] = malloc(cols * sizeof(double));
    for(i = 0; i < rows;++i)
        for(j = 0;j < cols;++j)
            output[i][j] = (double)rand()/(double)(RAND_MAX);
    return output;
}

Network *createNetwork(size_t numOfLayers, size_t *layerSizes){
    Network *out = malloc(sizeof(Network));
    out->layerSizes = malloc(sizeof(size_t) * numOfLayers);
    out->numOfLayers = numOfLayers;
    memcpy(out->layerSizes, layerSizes, sizeof(size_t) * numOfLayers);
    out->weights = malloc(sizeof(double **) * (numOfLayers-1));
    for (size_t i = 0; i < numOfLayers - 1; ++i){
        out->weights[i] = createMatrix(layerSizes[i]+1, layerSizes[i+1]);
    }
    return out;
}

void printVector(double *vm, size_t l){
    printMatrix(&vm, 1 , l);
}

void destroyNetwork(Network *network){
    for(size_t i = 0; i < network->numOfLayers-1; ++i){
        for(size_t j = 0; j < network->layerSizes[i]; ++j)
            free(network->weights[i][j]);
        free(network->weights[i]);
    }
    free(network->weights);
    free(network->layerSizes);
    free(network);
}

double *feedForward(Network *network, double *input){
    double *old = malloc(sizeof(double) * (network->layerSizes[0] + 1)) , *new;
    memcpy(old+1, input, sizeof(double) *  (network->layerSizes[0]));
    old[0] = 1;
    size_t i, j;
    for(i = 0; i < network->numOfLayers - 2; ++i){
        new = vectorbyMatrix(old, network->weights[i], network->layerSizes[i] + 1, network->layerSizes[i+1]);
        free(old);
        old = realloc(new, sizeof(double) * (network -> layerSizes [i+1]+1));
        for(size_t j = network->layerSizes[i+1]; j > 0; --j){
            old[j] = logistic(old[j-1]);
        }
        old[0] = 1;
    }
    new = vectorbyMatrix(old, network->weights[i], network->layerSizes[i] + 1, network->layerSizes[i+1]);
    for(j = 0; j < network->layerSizes[i+1]; ++j){
        new[j] = logistic(new[j]);
    }
    new = softmax(new, network->layerSizes[i+1]);
    free(old);
    return new;
}

double backPropagate(Network *network, double *input, double *expected, double learningRate){
    double **layers = malloc(sizeof(double *) * (network->numOfLayers-1));
    layers[0] = malloc(sizeof(double) * network->layerSizes[0] +1);
    memcpy(layers[0]+1, input, sizeof(double) * (network->layerSizes[0]));
    layers[0][0] = 1;
    size_t i,j,k;
    for(i = 0; i < network->numOfLayers-2; ++i){
        layers[i+1] = vectorbyMatrix(layers[i], network->weights[i], network->layerSizes[i]+1, network->layerSizes[i+1]);
        layers[i+1] = realloc(layers[i+1], sizeof(double) * (network->layerSizes[i+1]+1));
        for(j = network->layerSizes[i+1]; j > 0 ; --j){
            layers[i+1][j] = logistic(layers[i+1][j-1]);
        }
        layers[i+1][0] = 1;
    }
    layers[i+1] = vectorbyMatrix(layers[i], network->weights[i], network->layerSizes[i]+1, network->layerSizes[i+1]);
    for (j = 0 ; j < network -> layerSizes[i+1]; ++j)
      layers[i+1][j] = logistic(layers[i+1][j]);
    
    layers[i+1] = softmax(layers[i+1], network->layerSizes[i+1]);

    double **deltas = malloc(sizeof(double *) * (network->numOfLayers-1));
    deltas[i] = malloc(sizeof(double) * network->layerSizes[i+1]);
    for(j = 0; j < network->layerSizes[i+1]; ++j){
        deltas[i][j] = layers[i+1][j]*(1-layers[i+1][j]) * (expected[j] - layers[i+1][j]);
    }
    size_t bias = 0;
    for(--i; (int)i >= 0; --i){
        deltas[i] = malloc(sizeof(double) * (1+network->layerSizes[i+1]));
        for (j = 0; j < network->layerSizes[i+1]+1; ++j){
            deltas[i][j] = layers[i+1][j] * (1 - layers[i+1][j]) * (vectorMult(deltas[i+1],network->weights[i+1][j], network->layerSizes[i+2]+bias));
       }
       bias = 1;
    }
    for(i = 0; i < network->numOfLayers-1; ++i){
        for(j = 0; j < network->layerSizes[i]+1; ++j){
            for(k = 0; k < network->layerSizes[i+1]+bias;++k){
                network->weights[i][j][k] = network->weights[i][j][k] + learningRate*deltas[i][k] *layers[i][j];
                if(i == network->numOfLayers-2){
                    bias = 0;
                }
            }
        }
    }
}

double *softmax(double *a, size_t length){
    double sum = 0;
    for(size_t i = 0; i < length; ++i){
        sum += exp(a[i]);
    }
    double *new = malloc (sizeof(double) * length);
    for(size_t j = 0; j < length; ++j){
        new[j] = exp(a[j])/sum;
    }
    return new;
}

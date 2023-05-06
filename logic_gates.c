#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

// sigmoid activation function
float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

// define the training data for each logic gate
// each sample consists of two inputs (x1 and x2) and the expected output
typedef float sample[3];

sample or_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 1},
};

sample and_train[] = {
    {0, 0, 0},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 1},
};

sample nand_train[] = {
    {0, 0, 1},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample nor_train[] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
};

sample xor_train[] = {
    {0, 0, 0},
    {1, 0, 1},
    {0, 1, 1},
    {1, 1, 0},
};

sample sample_train[] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};

// number of iteration
int nb_loop = 1000*500;
// set the current training data and the number of samples
sample *train;
size_t train_count = 4;

// cost function - computes the mean squared error over the training data
float cost(float w1, float w2, float b) {
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = sigmoidf(x1*w1 + x2*w2 + b);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

// function to generate a random float between 0 and 1
float rand_float(void) {
    return (float) rand()/ (float) RAND_MAX;
}

// function to pick gate by input user
const char* choose_gate(void) {
    char* gate = malloc(10 * sizeof(char));
    printf("Choisissez la porte logique a entrainer :\n\
1. AND\n2. OR\n3. NOR\n4. NAND\n5. XOR\n6. XXX\n");
    scanf("%s", gate);
    return gate;
}

// function to have the gate index by it's name
int get_gate_index(const char* gate_name) {
    if (strcmp(gate_name, "AND") == 0) {
        return 0;
    } else if (strcmp(gate_name, "OR") == 0) {
        return 1;
    } else if (strcmp(gate_name, "NAND") == 0) {
        return 2;
    } else if (strcmp(gate_name, "NOR") == 0) {
        return 3;
    } else if (strcmp(gate_name, "XOR") == 0) {
        return 4;
    } else if (strcmp(gate_name, "XXX") == 0) {
        return 5;
    } else {
        printf("Porte logique invalide\n");
        exit(1);
    }
}

void fill_sample(sample s[])
{
    printf("Veuillez entrer 12 valeurs, chacune suivie d'un appui sur Entrée :\n");

    for (int i = 0; i < 4; i++) {
        printf("Ligne %d:\n", i+1);
        for (int j = 0; j < 3; j++) {
            printf("Valeur %d: ", j+1);
            scanf("%f", &s[i][j]);
        }
    }


}

int main(void) {
    const char* gate_name = choose_gate();
    int gate_index = get_gate_index(gate_name);
    free(gate_name);
    switch (gate_index) {
        case 0:
            train = and_train;
            break;
        case 1:
            train = or_train;
            break;
        case 2:
            train = nand_train;
            break;
        case 3:
            train = nor_train;
            break;
        case 4:
            train = xor_train;
            break;
        case 5:
            fill_sample(sample_train);
            train = sample_train;
    }
    // seed the random number generator with the current time
    srand(time(0));

    // initialize the weights and bias with random values between 0 and 1
    float w1 = rand_float();
    float w2 = rand_float();
    float b  = rand_float();

    // set the step size (epsilon) and learning rate
    float eps = 1e-1;
    float rate = 1e-1;

    // training loop
    for (size_t i = 0; i < nb_loop; ++i) {
        // compute the current cost and print the weights and bias
        float c = cost(w1, w2, b);
        printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, c);

        // compute the gradients for each weight and the bias using numerical differentiation
        float dw1 = (cost(w1 + eps, w2, b) - c)/eps;
        float dw2 = (cost(w1, w2 + eps, b) - c)/eps;
        float db  = (cost(w1, w2, b + eps) - c)/eps;

        // update the weights and bias using gradient descent
        w1 -= rate*dw1;
        w2 -= rate*dw2;
        b  -= rate*db;
    }
    printf("w1 = %f, w2 = %f, b = %f, c = %f\n", w1, w2, b, cost(w1, w2, b));

    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu | %zu = %f\n", i, j, sigmoidf(i*w1 + j*w2 + b));
        }
    }
    printf("Finished");
    return 0;
}

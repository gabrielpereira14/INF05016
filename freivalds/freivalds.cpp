#include <iostream>
#include <random>
#include <chrono>
#include <cassert>

typedef bool (* matrixComp)(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, const std::vector<std::vector<int>>& C);

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;


int FREIVALDS_ITERATIONS = 50;
int SIZE = (FREIVALDS_ITERATIONS * 3);



void matrixVectorMultiplication(const std::vector<std::vector<int>>& matrix, 
                                const std::vector<int>& vector, 
                                std::vector<int>& result) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; ++i) {
        result[i] = 0;
        for (int j = 0; j < cols; ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

void printVector(const std::vector<int>& vec){
    std::cout << "ABr = {";
    for (int i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << "}";

}


std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> distBin(0,1);

bool freivalds(const std::vector<std::vector<int>>& A, 
               const std::vector<std::vector<int>>& B, 
               const std::vector<std::vector<int>>& C) {

    static std::vector<int> r(SIZE);
    static std::vector<int> Br(SIZE, 0);
    static std::vector<int> ABr(SIZE, 0);
    static std::vector<int> Cr(SIZE, 0);

    int iterations = FREIVALDS_ITERATIONS;
    bool answer = true;

    while (iterations--)
    {
        for (int i = 0; i < SIZE; i++)
        {
            r[i] = distBin(rng);
        }
        
        
        matrixVectorMultiplication(B,r,Br);
        matrixVectorMultiplication(A,Br,ABr);
        matrixVectorMultiplication(C,r,Cr);

        for (size_t i = 0; i < SIZE; i++)
        {
            if((ABr[i] - Cr[i]) != 0){
                answer = false;
            } 
         
        }
    }
    return answer;
}

void mulMat(const std::vector<std::vector<int>>& mat1, 
            const std::vector<std::vector<int>>& mat2,
            std::vector<std::vector<int>>& result) {
    int size = mat1.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            result[i][j] = 0;
            for (int k = 0; k < size; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

bool matMultComp(const std::vector<std::vector<int>>& A, 
                 const std::vector<std::vector<int>>& B, 
                 const std::vector<std::vector<int>>& C) {

    int size = A.size();
    int tempSum = 0;

    bool anwser = true;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            tempSum = 0;
            for (int k = 0; k < size; k++) {
                tempSum += A[i][k] * B[k][j];
            }
            if(tempSum != C[i][j]) anwser = false;
        }
    }

    return true;
}

void generateRandomMatrix(std::vector<std::vector<int>>& mat, int n) {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(0, 100);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            mat[i][j] = distribution(generator);
        }
    }
}

void generateFullRandomInstance(std::vector<std::vector<int>>& A,std::vector<std::vector<int>>& B,std::vector<std::vector<int>>& C,int n) {
    generateRandomMatrix(A,n);
    generateRandomMatrix(B,n);
    generateRandomMatrix(C,n);
}

void generateCorrectRandomInstance(std::vector<std::vector<int>>& A,std::vector<std::vector<int>>& B,std::vector<std::vector<int>>& C,int n) {
    generateRandomMatrix(A,n);
    generateRandomMatrix(B,n);
    mulMat(A,B,C);
}

std::chrono::duration<long long, std::micro> getMatrixComparationTime(  const std::vector<std::vector<int>>& A, 
                                                                        const std::vector<std::vector<int>>& B,
                                                                        const std::vector<std::vector<int>>& C, 
                                                                        matrixComp function){
    auto t1 = high_resolution_clock::now();
    function(A,B,C);
    auto t2 = high_resolution_clock::now();
    return duration_cast<std::chrono::microseconds>(t2 - t1);
}

void printMatrix(const std::vector<std::vector<int>>& mat){
    for (const auto& row : mat) {
        for (const auto& elem : row) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char **argv){
    if (argc == 3)
    {
        FREIVALDS_ITERATIONS = atoi(argv[1]); 
        SIZE = atoi(argv[2]);
    }

    int size = SIZE; std::vector<std::vector<int>> A(size, std::vector<int>(size)), B(size, std::vector<int>(size)), C(size, std::vector<int>(size)); generateCorrectRandomInstance(A,B,C,size);
    C[0][0] += 1;
    int trueCounter = 0, falseCounter = 0, iterations = 100;
    for(int i = 0; i < iterations ; i++ ){
        freivalds(A,B,C) ? trueCounter++ : falseCounter++;
    }

    float successRate = 100 * (float)trueCounter/iterations; 
 
    std::cout << "Iterations: " << FREIVALDS_ITERATIONS << ", Size: " << SIZE << ", Trues: " << trueCounter << ", Falses: " << falseCounter << ", False positives percentage: " << successRate << "% " ;
}
 




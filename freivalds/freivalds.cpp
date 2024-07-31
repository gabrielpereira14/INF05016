#include <iostream>
#include <random>
#include <chrono>

typedef bool (* matrixComp)(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, const std::vector<std::vector<int>>& C);

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

#define FREIVALDS_ITERATIONS 50

void matrixVectorMultiplication(const std::vector<std::vector<int>>& matrix, 
                                const std::vector<int>& vector, 
                                std::vector<int>& result) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    for (int i = 0; i < rows; ++i) {
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

bool freivalds(const std::vector<std::vector<int>>& A, 
               const std::vector<std::vector<int>>& B, 
               const std::vector<std::vector<int>>& C) {

    int size = A.size();
    std::vector<int> r(size);
    std::vector<int> Br(size, 0);
    std::vector<int> ABr(size, 0);
    std::vector<int> Cr(size, 0);

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distBin(0,1);


    int iterations = FREIVALDS_ITERATIONS;
    bool answer = true;

    while (iterations--)
    {
        for (int i = 0; i < size; i++)
        {
            r[i] = distBin(rng);
        }
        
        
        matrixVectorMultiplication(B,r,Br);
        matrixVectorMultiplication(A,Br,ABr);
        matrixVectorMultiplication(C,r,Cr);

        for (size_t i = 0; i < size; i++)
        {
            if(ABr[i] - Cr[i] != 0){
                answer = false;
            } 
        }
    }

    return answer;
}

std::vector<std::vector<int>> mulMat(const std::vector<std::vector<int>>& mat1, 
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

    return result;
}

bool matMultComp(const std::vector<std::vector<int>>& A, 
                 const std::vector<std::vector<int>>& B, 
                 const std::vector<std::vector<int>>& C) {

    int size = A.size();
    std::vector<std::vector<int>> result(size, std::vector<int>(size, 0));
    mulMat(A,B,result);


    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            if(result[i][j] != C[i][j]) return false;
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

void generateRandomInstance(std::vector<std::vector<int>>& A,std::vector<std::vector<int>>& B,std::vector<std::vector<int>>& C,int n) {
    generateRandomMatrix(A,n);
    generateRandomMatrix(B,n);
    generateRandomMatrix(C,n);
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

int main(){
    //const int size = 9; const std::vector<std::vector<int>> A ={ {41, 67, 34, 0, 69, 24, 78, 58, 62 }, {64, 5, 45, 81, 27, 61, 91, 95, 42 }, {27, 36, 91, 4, 2, 53, 92, 82, 21 }, {16, 18, 95, 47, 26, 71, 38, 69, 12 }, {67, 99, 35, 94, 3, 11, 22, 33, 73 }, {64, 41, 11, 53, 68, 47, 44, 62, 57 }, {37, 59, 23, 41, 29, 78, 16, 35, 90 }, {42, 88, 6, 40, 42, 64, 48, 46, 5 }, {90, 29, 70, 50, 6, 1, 93, 48, 29 }}; const std::vector<std::vector<int>> B ={ { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}, { 1, 1, 1, 1, 1, 1, 1, 1, 1}}; const std::vector<std::vector<int>> C ={ {433, 433, 433, 433, 433, 433, 433, 433, 433 },{511, 511, 511, 511, 511, 511, 511, 511, 511 },{408, 408, 408, 408, 408, 408, 408, 408, 408 },{392, 392, 392, 392, 392, 392, 392, 392, 392 },{437, 437, 437, 437, 437, 437, 437, 437, 437 },{447, 447, 447, 447, 447, 447, 447, 447, 447 },{408, 408, 408, 408, 408, 408, 408, 408, 408 },{381, 381, 381, 381, 381, 381, 381, 381, 381 },{416, 416, 416, 415, 416, 416, 416, 416, 416 }};
    //const int size = 2;std::vector<std::vector<int>> A ={ {2, 3}, {3, 4}}; std::vector<std::vector<int>> B ={ { 1, 0}, { 1, 2}}; std::vector<std::vector<int>> C ={ {6, 5},{8, 7}};

    int size = FREIVALDS_ITERATIONS * 2; std::vector<std::vector<int>> A(size, std::vector<int>(size)), B(size, std::vector<int>(size)), C(size, std::vector<int>(size)); generateRandomInstance(A,B,C,size);

    int freivaldsAvg = 0;
    int matMulAvg = 0;

    int a = 50;

    for(int i = 0; i < a; i++){
        auto ms_int_freivalds = getMatrixComparationTime(A,B,C, freivalds);
        auto ms_int_matmul = getMatrixComparationTime(A,B,C, matMultComp);

        freivaldsAvg += ms_int_freivalds.count();
        matMulAvg += ms_int_matmul.count();
    }

    freivaldsAvg /= a; 
    matMulAvg /= a; 

    float ratio = (float)matMulAvg/(float)freivaldsAvg;

    std::cout << "Freivalds execution time: " << freivaldsAvg<< " Ms\n";
    std::cout << "Multiplication execution time: " << matMulAvg<< " Ms\n";
    std::cout << "Ratio: " << ratio<< " Ms\n";
}
 




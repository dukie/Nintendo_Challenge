#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cstring>
using namespace std;

#define ZERO_SIZE 1
#define MULTIPLIER_LENGTH 16


class AlphaCentauriNumber {
public:
    /*constructors and instantiators*/
    AlphaCentauriNumber(bool * n, int size);
    AlphaCentauriNumber();
    AlphaCentauriNumber(const AlphaCentauriNumber& other);


    static AlphaCentauriNumber zeroAlpha();
    ~AlphaCentauriNumber();
    /*getters and setters*/
    int getSize() {return size;};
    int getTMSB() {return tmsb;};

    bool * getDirect();;
    /*math*/
    bool operator==(AlphaCentauriNumber other);
    AlphaCentauriNumber operator+(AlphaCentauriNumber other);
    AlphaCentauriNumber operator*(AlphaCentauriNumber other);
    AlphaCentauriNumber operator/(AlphaCentauriNumber other);
    AlphaCentauriNumber operator=(AlphaCentauriNumber other);
    AlphaCentauriNumber operator%(AlphaCentauriNumber other);
    bool operator[](int idx);
    AlphaCentauriNumber findDelimeter(AlphaCentauriNumber other);
    void findTMSB();
    /*other*/
    string toString(int sizeOfOutput);
private:
    bool * num;
    int size;
    int tmsb;
};


bool * createBoolArray(int size) {

    bool* n = new bool[size];
    memset(n, false, (size_t) size);
    return n;
}


AlphaCentauriNumber::AlphaCentauriNumber() {
    size = 1;
    this->num = createBoolArray(1);
    this->num[0] = true;
    this->findTMSB();
}


AlphaCentauriNumber::AlphaCentauriNumber(bool * n, int size) {
    this->size = size;
    this->num = n;
    this->findTMSB();
}


AlphaCentauriNumber::AlphaCentauriNumber(const AlphaCentauriNumber& other) {
    size = other.size;
    tmsb = other.tmsb;
    num = createBoolArray(size);
    for (int i = 0; i < size; i++) {
        num[i] = other.num[i];
    }
}


AlphaCentauriNumber AlphaCentauriNumber::zeroAlpha() {

    bool * n = createBoolArray(ZERO_SIZE);
    return AlphaCentauriNumber(n,ZERO_SIZE);

}


AlphaCentauriNumber::~AlphaCentauriNumber() {
    delete[] num;
}


AlphaCentauriNumber fromInput(int size) {
    bool * n = createBoolArray(size*2);
    int tmpStoreLen = size / MULTIPLIER_LENGTH;
    unsigned int store;
    for (int i = 0; i < tmpStoreLen; i++) {
        cin >> hex >> store;
        for (int j=0; j < 32; j++){
            n[32*i+j] = (bool) (store & 1);
            store >>= 1;
        }

    }
    return AlphaCentauriNumber(n, size*2);
}


bool * AlphaCentauriNumber::getDirect(){
    return this->num;
}


string AlphaCentauriNumber::toString(int sizeOfOutput){
    stringstream ss;
    int numCount = sizeOfOutput / 32;
    for (int i=0; i < numCount; i++) {
        unsigned int partial = 0;
        for (int j = 0; j < 32; j++) {
            partial += (*this)[32*i+j] << j;
        }
        ss << hex << setfill('0') << setw(8) << partial;
        if(numCount > 1 && numCount-1 !=i)
            ss << " ";
    }

    return ss.str();
}

/*ADJUST*/
void AlphaCentauriNumber::findTMSB() {

    for(int i = this->size -1 ; i >= 0 ; i--) {
        if (this->num[i]) {
            this->tmsb = i;
            break;
        }
        else {
            this->tmsb = -1;
        }
    }
}


/* MATH*/
AlphaCentauriNumber AlphaCentauriNumber::findDelimeter(AlphaCentauriNumber other) {
    if(tmsb < other.getTMSB())
        return other.findDelimeter(*this);

    AlphaCentauriNumber module = (*this) % other;
    if (module == AlphaCentauriNumber::zeroAlpha())
        return other;
    return other.findDelimeter(module);
}


AlphaCentauriNumber AlphaCentauriNumber::operator=(AlphaCentauriNumber other){
    bool * tmp;
    tmp = this->num;
    this->num = other.num;
    other.num = tmp;
    this->size = other.size;
    this->tmsb = other.tmsb;
    return *this;
}


bool AlphaCentauriNumber::operator[](int idx) {
    if(idx > tmsb || num[idx] > 1){
        return false;
    }
    return num[idx];
}


AlphaCentauriNumber AlphaCentauriNumber::operator+(AlphaCentauriNumber other) {
    AlphaCentauriNumber that = *this;
    if(that.getTMSB() < other.getTMSB())
        return other + that;

    int newSize = that.getTMSB() + 1;
    bool* n  = createBoolArray(newSize);

    for(int i = 0; i < newSize; i++) {
        n[i] = that[i] ^ other[i];
    }
    return AlphaCentauriNumber(n, newSize);
}


AlphaCentauriNumber AlphaCentauriNumber::operator*(AlphaCentauriNumber other) {
    //int tmsbFirst = tmsb;
    int tmsbSecond = other.getTMSB();

    int newSize = tmsb+tmsbSecond+2;
    bool* n = createBoolArray(newSize);

    bool* first = this->getDirect();
    bool* second = other.getDirect();

    for (int i = 0; i <= tmsb; i++) {
        for (int k = 0; k <= tmsbSecond; k++) {
            n[i+k] ^= first[i] & second[k];
        }
    }

    return AlphaCentauriNumber(n,newSize);

}


bool AlphaCentauriNumber::operator==(AlphaCentauriNumber other) {
    int firstTmsb = tmsb;
    int secondTmsb = other.getTMSB();

    if(firstTmsb != secondTmsb) {
        return false;
    }
    for(int i = 0; i <= firstTmsb; i++){
        if ((*this)[i] != other[i])
            return false;
    }

    return true;
}


AlphaCentauriNumber AlphaCentauriNumber::operator/(AlphaCentauriNumber other) {

    AlphaCentauriNumber that = *this;
    int firstTmsb = that.getTMSB();
    int secondTmsb = other.getTMSB();
    if(firstTmsb < secondTmsb) {
        return AlphaCentauriNumber::zeroAlpha();
    }

    int oneBitArrLen = firstTmsb - secondTmsb + 1;
    bool * oneBitAlpha = createBoolArray(oneBitArrLen);
    oneBitAlpha[firstTmsb - secondTmsb] = true;
    AlphaCentauriNumber oneBitANum = AlphaCentauriNumber(oneBitAlpha, oneBitArrLen);
    return oneBitANum + (that + other * oneBitANum) / other;

}


AlphaCentauriNumber AlphaCentauriNumber::operator%(AlphaCentauriNumber other) {

    bool* n = createBoolArray(tmsb+1);
    for(int i = 0; i <= tmsb; i++) {
        n[i] = num[i];
    }

    int nTMSB = tmsb;
    bool * otherDirect = other.getDirect();
    int secondTMSB = other.getTMSB();
    while ((nTMSB>=0) && (secondTMSB > 0)) {
        if (nTMSB < secondTMSB)
            return AlphaCentauriNumber(n,nTMSB+1);
        int dif = nTMSB - secondTMSB;

        for (int i = 0; i <= secondTMSB; i++){
            n[i+dif] ^= otherDirect[i];
        }

        for (int i = nTMSB; i >= 0; --i) {
            if (n[i]) {
                break;
            }
            n[nTMSB] = false;
            nTMSB--;
        }
    }
    delete[] n;
    return AlphaCentauriNumber::zeroAlpha();
}


void exchange(bool *a, bool *b) {
    bool tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

bool compare (string one, string two) {
    return (one==two);
}


void shiftMatrix(int baseTMSB, bool* matrix, bool* identityMatrix) {
    int fixedY = 0;
    int i;
    for(int row = 0; row < baseTMSB; row++) {
        int toeHold = 0;
        bool point = false;
        for (toeHold = fixedY; toeHold < baseTMSB; toeHold++) {
            point = matrix[row * baseTMSB + toeHold];
            if (point)
                break;
        }

        if (!point)
            continue;
        int idx1 = 0;
        int idx2 = 0;
        for(i =0; i < baseTMSB; i++) {
            idx1 = i * baseTMSB + fixedY;
            idx2 = i * baseTMSB + toeHold;
            exchange(&matrix[idx1], &matrix[idx2]);
            exchange(&identityMatrix[idx1], &identityMatrix[idx2]);
        }


        for (int j = fixedY + 1; j < baseTMSB; j++) {
            if (matrix[row * baseTMSB + j]) {
                for (i =0 ; i < baseTMSB; i++) {
                    idx1 = i * baseTMSB + j;
                    idx2 = i * baseTMSB + fixedY;
                    matrix[idx1] ^=  matrix[idx2];
                    identityMatrix[idx1] ^= identityMatrix[idx2];
                }
            }
        }
        fixedY++;
    }
}


vector<AlphaCentauriNumber> partial(AlphaCentauriNumber ANUM) {

    vector<AlphaCentauriNumber> baseVec;
    int baseTMSB = ANUM.getTMSB() +1;
    bool *matrix = createBoolArray(baseTMSB * baseTMSB);
    bool *identityMatrix = createBoolArray(baseTMSB * baseTMSB);
    for (int i =0; i< baseTMSB; i++) {
        identityMatrix[i * baseTMSB + i] = true;
    }


    for(int index = 0; index< baseTMSB; index++){

        bool * oneBitAlpha = createBoolArray(index+1);
        oneBitAlpha[index] = true;
        AlphaCentauriNumber roundACN = AlphaCentauriNumber(oneBitAlpha, index+1);
        AlphaCentauriNumber sqrRoundACN =  roundACN * roundACN % ANUM;
        int sqrRACNSize = sqrRoundACN.getSize();
        bool * directRACN = sqrRoundACN.getDirect();
        int idx = index-1 < 0 ? baseTMSB-1 : index-1;
        for(int i = 0; i< sqrRACNSize; i++)
            matrix[i * baseTMSB + idx] = directRACN[i];
        matrix[idx * baseTMSB + idx] ^= 1;
    }

    shiftMatrix(baseTMSB, matrix, identityMatrix);

    for (int j = baseTMSB-1; j >= 0; j--){

        bool flag = false;
        for(int m = 0; m < baseTMSB; m++)  {
            flag ^= matrix[m * baseTMSB + j];
        }
        if(!flag) {
            bool * n = createBoolArray(baseTMSB);
            for(int m = 0; m < baseTMSB; m++)  {
                n[m] = identityMatrix[m * baseTMSB + j];
            }
            baseVec.insert(baseVec.end(), AlphaCentauriNumber(n,baseTMSB));
        }
        else {
            delete [] matrix;
            delete [] identityMatrix;
            break;
        }
    }

    return baseVec;
}

vector<AlphaCentauriNumber> findMultipliers(AlphaCentauriNumber ANUM) {

    vector<AlphaCentauriNumber> possibles = partial(ANUM);
    for(vector<AlphaCentauriNumber>::iterator it = possibles.begin(); it != possibles.end(); ++it) {

        if ((*it).getTMSB() >= 1){
            AlphaCentauriNumber multiplier = ANUM.findDelimeter((*it));

            if(!(AlphaCentauriNumber() == multiplier) && multiplier.getTMSB() < ANUM.getTMSB()){
                vector<AlphaCentauriNumber> left = findMultipliers(multiplier);
                vector<AlphaCentauriNumber> right = findMultipliers(ANUM/multiplier);
                left.insert(left.end(), right.begin(), right.end());
                return left;
            }
        }


    }

    vector<AlphaCentauriNumber> sourceANUM;
    sourceANUM.insert(sourceANUM.end(), ANUM);
    return sourceANUM;
}


void printResults(vector<AlphaCentauriNumber> numsVector, int size) {

    vector<vector<AlphaCentauriNumber> > combinations;

    AlphaCentauriNumber source;

    for(vector<AlphaCentauriNumber>::size_type i = 0; i < numsVector.size(); i++) {
        source = numsVector[i] * source;

        for(vector<AlphaCentauriNumber>::size_type k = 0; k < numsVector.size(); k++) {
            if (i == k)
                continue;

            vector<AlphaCentauriNumber> entry;
            entry.insert(entry.end(), numsVector[k]);
            entry.insert(entry.end(), numsVector[i]);
            combinations.insert(combinations.end(), entry);
        }
    }

    vector<AlphaCentauriNumber> first;
    vector<AlphaCentauriNumber> last;
    first.insert(first.end(),AlphaCentauriNumber() );
    first.insert(first.end(), source);
    combinations.insert(combinations.begin(),first);
    last.insert(last.end(), source);
    last.insert(last.end(), AlphaCentauriNumber());
    combinations.insert(combinations.end(),last);

    vector<string> resOut;
    for(vector<AlphaCentauriNumber>::size_type i = 0; i < combinations.size(); i++) {
        stringstream ss;
        if( (combinations[i][0].getTMSB()) < size && (combinations[i][1].getTMSB() < size))
            ss << combinations[i][0].toString(size) << " " <<  combinations[i][1].toString(size) << endl;
        resOut.insert(resOut.end(),ss.str());
    }
    unique(resOut.begin(), resOut.end(), compare);
    sort(resOut.begin(), resOut.end());
    for(vector<AlphaCentauriNumber>::size_type i = 0; i < resOut.size(); i++) {
        cout << resOut[i];
    }



}

int main()
{
    unsigned int size;
    cin >> size;
    AlphaCentauriNumber num = fromInput(size);
    printResults(findMultipliers(num), size);
    return 0;
}

#include <iostream>
#include <string>
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
        bool get(std::size_t idx);
        bool * getDirect();
        void set(std::size_t idx, bool val) { num[idx] = val; };
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
	memset(n, false, size);
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
			n[32*i+j] = store & 1;
			store >>= 1;
		}
		
	}
	return AlphaCentauriNumber(n, size*2);
}

AlphaCentauriNumber fromString(stringstream* input, int size) {
	
	bool * n = createBoolArray(size*2);
	int tmpStoreLen = size / MULTIPLIER_LENGTH;
	unsigned int store;
	for (int i = 0; i < tmpStoreLen; i++) { 
		*input >> hex >> store;
		for (int j=0; j < 32; j++){
			n[32*i+j] = store & 1;
			store >>= 1;
		}
		
	}
	return AlphaCentauriNumber(n, size*2);
}


/*SET GET*/

bool AlphaCentauriNumber::get(std::size_t idx) {
	if((int)idx > this->getTMSB()) {
		return false;
	}
	return num[idx]; 
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
		if (this->num[i] == true) {
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
	int tmsbFirst = tmsb;
	int tmsbSecond = other.getTMSB();
	
	int newSize = tmsbFirst+tmsbSecond+2;
	bool* n = createBoolArray(newSize);
	
	bool* first = this->getDirect();
	bool* second = other.getDirect();
	
	for (int i = 0; i <= tmsbFirst; i++) {
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



AlphaCentauriNumber createFromInt(unsigned int num) {

	bool* n = createBoolArray(32);
	for (int j=0; j < 32; j++){
			n[j] = num & 1;
			num >>= 1;
		}
		
	return AlphaCentauriNumber(n, 32);
}

bool** createZero2DArr(int size) {
	bool **matrix = new bool*[size];
	for(int i = 0; i < size; i++) {
		matrix[i] = createBoolArray(size);
	}
	return matrix;
}

void delete2dArr(bool** arr, int size) {
	for(int i = 0; i < size; i++) {
		delete [] arr[i];
	}
	delete [] arr;
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

vector<AlphaCentauriNumber> findVec(AlphaCentauriNumber ANUM) {
	
	vector<AlphaCentauriNumber> baseVec;
	int baseTMSB = ANUM.getTMSB() +1;
	bool **matrix = createZero2DArr(baseTMSB);
	bool **singleMatrix = createZero2DArr(baseTMSB);
	for (int i =0; i< baseTMSB; i++) {
			singleMatrix[i][i] = true;
	}
	
	
	for(int index = 0; index< baseTMSB; index++){
		
		bool * oneBitAlpha = createBoolArray(index+1);
		oneBitAlpha[index] = true;
		AlphaCentauriNumber roundACN = AlphaCentauriNumber(oneBitAlpha, index+1);
		AlphaCentauriNumber sqr = roundACN * roundACN;
		AlphaCentauriNumber sqrRoundACN =  sqr % ANUM;
		int sqrRACNSize = sqrRoundACN.getSize();
		bool * directRACN = sqrRoundACN.getDirect();
		int idx = index-1 < 0 ? baseTMSB-1 : index-1;
		for(int i = 0; i< sqrRACNSize; i++)
			matrix[i][idx] = directRACN[i];
		matrix[idx][idx] ^= 1;
	}
	int fixedY = 0;
	for(int row = 0; row < baseTMSB; row++) {
		int toeHold = 0;
		for (toeHold = fixedY; toeHold< baseTMSB; toeHold++) {
			if (matrix[row][toeHold])
				break;
		}

		if (!matrix[row][toeHold])
			continue;
		for(int i =0; i < baseTMSB; i++) {
			exchange(&matrix[i][fixedY], &matrix[i][toeHold]);
			exchange(&singleMatrix[i][fixedY], &singleMatrix[i][toeHold]);
		}
		
		
		for (int j = fixedY + 1; j < baseTMSB; j++) {
			if (matrix[row][j]) {
				for (int i =0 ; i < baseTMSB; i++) {
					matrix[i][j] ^=  matrix[i][fixedY];
					singleMatrix[i][j] ^= singleMatrix[i][fixedY];
				}
			}
		}
		fixedY++;	
	}
	
	
	for (int j = baseTMSB-1; j >= 0; j--){
		
		bool flag = false;
		for(int m = 0; m < baseTMSB; m++)  {
			flag ^= matrix[m][j];
		}
		if(!flag) {
			bool * n = createBoolArray(baseTMSB);
			for(int m = 0; m < baseTMSB; m++)  {
				n[m] = singleMatrix[m][j];
			}
			baseVec.insert(baseVec.end(), AlphaCentauriNumber(n,baseTMSB));
		}
		else {
			delete2dArr(matrix, baseTMSB);
			delete2dArr(singleMatrix, baseTMSB);
			break;
		}
	}

	return baseVec;
}

vector<AlphaCentauriNumber> findMultipliers(AlphaCentauriNumber ANUM) {

	vector<AlphaCentauriNumber> possibles = findVec(ANUM);
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


void printResults(vector<AlphaCentauriNumber> numsVector, int size, AlphaCentauriNumber base){
	
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
	
	if(source.getTMSB() < size) {
			vector<AlphaCentauriNumber> first;
			vector<AlphaCentauriNumber> last;
			first.insert(first.end(),AlphaCentauriNumber() );
			first.insert(first.end(), source);
			combinations.insert(combinations.begin(),first);
			last.insert(last.end(), source);
			last.insert(last.end(), AlphaCentauriNumber());
			combinations.insert(combinations.end(),last);
		}
	vector<string> resOut;
	for(vector<AlphaCentauriNumber>::size_type i = 0; i < combinations.size(); i++) {
		stringstream ss;
		if( (combinations[i][0] * combinations[i][1]) == base)
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
	printResults(findMultipliers(num), size, num);
	return 0;
}



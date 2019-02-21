template <class DT>
class ExceptionAdd{};

template <class DT>
class ExceptionMultiply{};

template <class DT>
class ExceptionCV{};

template <class DT> 
class SparseRow {
protected:
	int row, col; // row and column index
	DT value; // the element's value
public:
	SparseRow();
	SparseRow(int r, int c, DT& v);
	virtual ~SparseRow();
	void display();
	int getRow();
	int getCol();
	DT getValue();
};

template <class DT>
class SparseMatrix {
protected:
	int noRows, noCols;	// #rows and #cols in the original matrix
	int	commonValue; // the common value in the original matrix
	int	noNonSparseValues; // # of uncommon values in the matrix
	vector<SparseRow<DT>>* myMatrix; // sparse matrix containing uncommon elements
public:
	SparseMatrix();
	SparseMatrix(int n, int m, int cv);
	virtual ~SparseMatrix();

	void read();
	// void sort();
	void setSparseRow(int pos, int r, int c, DT& v);
	void displayMatrix();

	vector<SparseRow<DT>>* getMatrix();
	int getCols();
	int getRows();

	int valueOf(int r, int c);
	bool add(int r, int c, int val);

	SparseMatrix<DT> operator*(SparseMatrix<DT> M);
	SparseMatrix<DT> operator+(SparseMatrix<DT> M);
	SparseMatrix<DT> operator!();
	friend ostream operator<<(ostream& s);
};

#include <iostream>
#include <vector>
using namespace std;

template <class DT>
SparseRow<DT>::SparseRow() {
	row = -1; col = -1;
	value = 0; // should this be null?
}

template <class DT>
SparseRow<DT>::SparseRow(int r, int c, DT& v) {
	row = r; col = c;
	value = v;
}

template <class DT>
SparseRow<DT>::~SparseRow() {
	delete value;
}

template <class DT>
void SparseRow<DT>::display() {
	cout << row << ", " << col << ", " << value << endl;
}

template <class DT>
int SparseRow<DT>::getRow() {
	return row;
}

template <class DT>
int SparseRow<DT>::getCol() {
	return col;
}

template <class DT>
DT SparseRow<DT>::getValue() {
	return value;
}

template <class DT>
SparseMatrix<DT>::SparseMatrix() {
	noRows = -1, noCols = -1, commonValue = 0;
	myMatrix = new vector<SparseRow<DT>>();
}

template <class DT>
SparseMatrix<DT>::SparseMatrix(int n, int m, int cv) {
	noRows = n, noCols = m, commonValue = cv;
	myMatrix = new vector<SparseRow<DT>>();
}

template <class DT>
SparseMatrix<DT>::~SparseMatrix() {
	delete[] myMatrix;
}
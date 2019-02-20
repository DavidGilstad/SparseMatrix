/*
template <class DT>
class ExceptionAdd {};

template <class DT>
class ExceptionMultiply {};

template <class DT>
class ExceptionCV {};



template <class DT> class SparseRow {
protected:
	int row, col; // row and column index
	DT value; // the element's value

public:
	SparseRow ();
	SparseRow(int r, int c, DT& v);
	virtual ~SparseRow();
	void display();

	// added methods
	int getRow();
	int getCol();
	DT getVal();
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

	SparseMatrix<DT> operator*(SparseMatrix<DT> M);
	SparseMatrix<DT> operator+(SparseMatrix<DT> M);
	SparseMatrix<DT> operator!();
	SparseMatrix<DT> operator<<();

	void setSparseRow(int pos, int r, int c, DT& v);
	void display();
	void displayMatrix();
};




template <class DT>
SparseMatrix<DT>::SparseMatrix(int n, int m, int cv) {
	int noRows = n;
	int noCols = m;
	int commonValue = cv;
	// noNonSparseValues = noNSV;
	// myMatrix = new SparseRow[noNSV];
}
*/
// given code for project 2

/*
 * Make fixes to noNSV where necessary
 *		- fix read in method
 *		- myMatrix needs different initialization (later)
 *
 * Overload operators ( *, +, !, << )
 *
 * Create exceptions for multiplication and addition
 *
 * Use DT template
 *
 * Implement myMatrix
 *
 * Create destructors for sparse row and matrix
 */

class SparseRow {
protected:
	int row, col, value; // row and column index, and the element's value
public:
	SparseRow();
	int getRow();
	int getCol();
	int getValue();
	void add(int val);
	void display();
	void set(int r, int c, int val);
};

class SparseMatrix {
	friend SparseMatrix* operator* (const SparseMatrix &x, const SparseMatrix &y);
	friend SparseMatrix* operator+ (const SparseMatrix &x, const SparseMatrix &y);
	friend SparseMatrix* operator! (const SparseMatrix &x);

protected:
	int noRows, noCols;	// #rows and #cols in the original matrix
	int	noNonSparseValues; // # of uncommon values in the matrix
	int	commonValue; // the common value in the original matrix
	SparseRow* myMatrix; // sparse matrix containing uncommon elements
public:
	SparseMatrix();
	SparseMatrix(int n, int m, int cv, int noNSV);
	~SparseMatrix();
	int getCols();
	int getRows();
	int valueOf(int r, int c);
	bool add(int r, int c, int val, int size);
	SparseRow* getMatrix();
	SparseMatrix* Transpose();
	SparseMatrix* Multiply(SparseMatrix& M);
	SparseMatrix* Add(SparseMatrix& M);
	void sort();
	void display();
	void displayMatrix();
	void read();
};

SparseMatrix* operator* (SparseMatrix &x, SparseMatrix &y) {
	SparseMatrix* product = x.Multiply(y);
	return product;
}

SparseMatrix* operator+ (SparseMatrix &x, SparseMatrix &y) {
	SparseMatrix* sum = x.Add(y);
	return sum;
}

SparseMatrix* operator! (SparseMatrix &x) {
	return x.Transpose();
}

SparseRow::SparseRow() {
	row = -1, col = -1, value = 0;
}

SparseMatrix::SparseMatrix() {
	noRows = -1, noCols = -1, commonValue = 0, noNonSparseValues = 0;
	myMatrix = new SparseRow[0];
}

SparseMatrix::SparseMatrix(int n, int m, int cv, int noNSV) {
	noRows = n, noCols = m, commonValue = cv, noNonSparseValues = noNSV;
	myMatrix = new SparseRow[noNSV];
}

SparseMatrix::~SparseMatrix() {
	delete myMatrix;
}

#include <iostream>
using namespace std;

int SparseRow::getRow() {
	return row;
}

int SparseRow::getCol() {
	return col;
}

int SparseRow::getValue() {
	return value;
}

void SparseRow::add(int val) {
	value += val;
}

void SparseRow::display() {
	cout << row << ", " << col << ", " << value << endl;
}

/*
 * Sets row, col, and value to r, c, and val respectively.
 */
void SparseRow::set(int r, int c, int val) {
	row = r, col = c, value = val;
}

int SparseMatrix::getCols() {
	return noCols;
}

int SparseMatrix::getRows() {
	return noRows;
}

int SparseMatrix::valueOf(int r, int c) {
	for (int i = 0; i < noNonSparseValues; i++)
		if (myMatrix[i].getRow() == r && myMatrix[i].getCol() == c)
			return myMatrix[i].getValue();
	return commonValue;
}

bool SparseMatrix::add(int r, int c, int val, int size) {
	for (int i = 0; i < size; i++)
		if (myMatrix[i].getRow() == r && myMatrix[i].getCol() == c) {
			myMatrix[i].add(val);
			return true;
		}
	return false;
}

SparseRow* SparseMatrix::getMatrix() {
	return myMatrix;
}

SparseMatrix* SparseMatrix::Transpose() {
	SparseMatrix* transposed = // flipped # of rows and columns
		new SparseMatrix(noCols, noRows, commonValue, noNonSparseValues);

	for (int i = 0; i < noNonSparseValues; i++) { // flip column and row index
		SparseRow* t = &(*this).getMatrix()[i];
		(*transposed).getMatrix()[i].set((*t).getCol(), (*t).getRow(), (*t).getValue());
	}
	return transposed;
}

SparseMatrix* SparseMatrix::Multiply(SparseMatrix& M) {
	if (noCols != M.getRows()) {
		cout << "Error: Matrices have invalid size for multiplication." << endl;
		return new SparseMatrix();
	} // #cols of the first must be equal to #rows of the second matrix

	SparseMatrix* P = new SparseMatrix(noRows, M.getCols(), 0, noRows*M.getCols());
	int v, pSize = 0;

	for (int i = 0; i < noNonSparseValues; i++)
		// go through each element and try to find multplication match in M
		for (int mCol = 0; mCol < M.noCols; mCol++) {
			v = myMatrix[i].getValue() * M.valueOf(myMatrix[i].getCol(), mCol);
			// if match in M exists, put the product in product matrix P
			if (!(*P).add(myMatrix[i].getRow(), mCol, v, pSize) && v != 0)
				(*P).getMatrix()[pSize++].set(myMatrix[i].getRow(), mCol, v);
		}
	(*P).noNonSparseValues = pSize;
	return P;
}

SparseMatrix* SparseMatrix::Add(SparseMatrix& M) {
	if (noRows != M.getRows() || noCols != M.getCols()) {
		cout << "Error: Matrices have invalid size for addition" << endl;
	} // matrices must have equal sizes

	SparseMatrix* S = new SparseMatrix(noRows, noCols, commonValue, noRows*noCols);
	SparseRow t;	// temporary sparse row holder
	int sSize = 0;	// tracks used elements in S

	// assume all nonspares elements don't have a matching nonsparse value in M
	for (int i = 0; i < noNonSparseValues; i++) {
		t = (*this).getMatrix()[i];
		(*S).getMatrix()[sSize++].set( // add common value from M
			t.getRow(), t.getCol(), t.getValue() + M.commonValue);
	}
	for (int i = 0; i < M.noNonSparseValues; i++) {
		t = M.getMatrix()[i];
		// if M has matching nonsparse value, subtract common value added above
		if (!(*S).add(t.getRow(), t.getCol(), t.getValue() - commonValue, sSize))
			(*S).getMatrix()[sSize++].set( // if no match, add common value
				t.getRow(), t.getCol(), t.getValue() + commonValue);
	}
	(*S).noNonSparseValues = sSize;
	return S;
}

void SparseMatrix::sort() {
	SparseRow temp;
	for (int i = 0; i < noNonSparseValues; i++)
		for (int j = i; j > 0; j--) {
			int r1 = myMatrix[j].getRow(), c1 = myMatrix[j].getCol(),
				r2 = myMatrix[j - 1].getRow(), c2 = myMatrix[j - 1].getCol();
			if (r1 < r2 || r1 == r2 && c1 < c2) {
				temp = myMatrix[j];
				myMatrix[j] = myMatrix[j - 1];
				myMatrix[j - 1] = temp;
			}
			else j = 0;
		}
}

void SparseMatrix::display() {
	for (int i = 0; i < noNonSparseValues; ++i)
		myMatrix[i].display();
}

void SparseMatrix::displayMatrix() {
	for (int row = 0; row < noRows; row++) {
		for (int col = 0; col < noCols; col++) {
			cout << (*this).valueOf(row, col) << "\t";
		} cout << endl;
	}
}

void SparseMatrix::read() {
	int val, i = 0;
	for (int row = 0; row < noRows; ++row)
		for (int col = 0; col < noCols; ++col) {
			cin >> val;
			if (val != commonValue)
				myMatrix[i++].set(row, col, val);
		}
	noNonSparseValues = i;
}

int main() {
	int n, m, cv, noNSV;
	SparseMatrix* temp;

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* firstOne = new SparseMatrix(n, m, cv, noNSV);
	(*firstOne).read();

	cin >> n >> m >> cv >> noNSV;
	SparseMatrix* secondOne = new SparseMatrix(n, m, cv, noNSV);
	(*secondOne).read();

	cout << "First one in sparse matrix format" << endl;
	(*firstOne).display();

	cout << "First one in normal matrix format" << endl;
	(*firstOne).displayMatrix();

	cout << "Second one in sparse matrix format" << endl;
	(*secondOne).display();

	cout << "Second one in normal matrix format" << endl;
	(*secondOne).displayMatrix();

	cout << "After Transpose first one in normal format" << endl;
	temp = !(*firstOne);
	(*temp).displayMatrix();

	cout << "After Transpose second one in normal format" << endl;
	temp = !(*secondOne);
	(*temp).displayMatrix();

	cout << "Multiplication of matrices in sparse matrix form:" << endl;
	temp = (*secondOne)*(*firstOne);
	(*temp).sort();
	(*temp).display();

	cout << "Addition of matrices in sparse matrix form:" << endl;
	temp = (*secondOne)+(*firstOne);
	(*temp).sort();
	(*temp).display();

	system("pause");
	return 0;
}
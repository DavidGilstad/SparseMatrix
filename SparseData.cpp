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

template <class DT>
SparseRow<DT>::SparseRow() {
	row = -1; col = -1;
	value = null;
}

template <class DT>
SparseRow<DT>::SparseRow(int r, int c, DT& v) {
	row = r; col = c;
	value = v;
}
/*------------------------------------------------------------------------------------------\
@ Numerical Methods by Young-Keun Kim - Handong Global University

Author          : Kyung-Min Lee
Created         : 10-03-2019
Modified        : 06-04-2019
Language/ver	: C in MSVS2017
Course			: Numerical method 2019-Spring

Description     : Assignment 4
/------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "myNM.h"
#include "myMatrix.h"


/*--------------------------------------------------------------------------*/
/*							Non-linear Solver 								*/
/*--------------------------------------------------------------------------*/


/* Function
	_x		: variable */
double func(double _x)
{
	double m = 2500;											// Setting the given constants
	double k = 300000;
	double c = 36000;

	// Function from the given formula
	return pow(_x, -1) - 2;													// 2nd function
	//4 * pow(m,2) *pow(_x, 4) - (8 * k * m + 21 * pow(c, 2)) * pow(_x, 2) - 21 * pow(k, 2);	=> 1st function
}

/* Differential Function
	_x		: variable */
double dfunc(double _x)
{
	double m = 2500;											// Setting the given constants
	double k = 300000;
	double c = 36000;

	// First derivation of the func.
	return -1 * pow(_x, -2);									// 2nd function
	//16 * pow(m,2) * pow(_x, 3) - (16 * k * m + 42 * pow(c, 2)) * _x;							=> 1st function
}

/* Bisection Method
	_a		: initial value #1
	_b		: initial value #2
	_tol	: tolerance	
	
	Cannot find true value when...
	1) f(x_true) == 0
	2) f(a) * f(b) > 0
	*/
double bisectionNL(float _a, float _b, float _tol)
{
	int i, MaxIt = 30;								// Set the maximum iteration number
	double x_ns;

	for (i = 0; i < MaxIt; i++)						// Iterate until MaxIteration.
	{
		
		if (func(_a)*func(_b) < 0)				// Check whether the interval is reasonable.
		{
			x_ns = (_a + _b) / 2;				// Find the midpoint of a and b
			double tol = fabs((_b - _a)/2);
			
			printf("Iteration:%3d	Left:%11.8lf   Right:%11.8lf   x_NS:%11.8lf   Tolerance: %11.8lf\n", i, _a, _b, x_ns, tol);
			

			if (tol < _tol)						// Check whether x_ns satisfies tolerance. 
			{
				break;
			}
			else
			{
				if (func(_a) * func(x_ns) < 0)
				{
					_b = x_ns;									// case 1. x_true is in between a & x_ns
				}
				else
				{
					_a = x_ns;									// case 2. x_true is in between x_ns & b
				}
			}
		}
		else
		{
			printf("True value(n_T) does not exist between 'a' and 'b' !!\n");
			break;
		}
		
	}
	
	if (i == MaxIt)												// When the solution is not found within the given number of iterations
	{
		printf("The solution is not found within the given number of iterations!! \n");  // Show the n_true is found within the max iteration.
		return x_ns;
	}
	else 
	{
		printf("The solution is found within the given number of iteration!! \n");
		return x_ns;
	}
	
}

/* Newton Raphson Method
	_x0		: Initial value
	_tol	: tolerance	*/
double newtonRaphson(float _x0, float _tol)
{
	double tol, x_NS, x_c = _x0;
	int i = 0, MaxIt = 30;								// Set the maximum iteration number

	while (i <= MaxIt)
	{
		x_NS = x_c - (func(x_c) / dfunc(x_c));			// Calculating derivative and applying 
		tol = (fabs(x_NS - x_c) / fabs(x_c));			// tolerance for NR Method
		printf("Iteration:%3d	x_NS: %11.8lf	tolerance:%11.8lf \n", i, x_NS, tol);

		if (dfunc(x_c) == 0)							// When dfunc = 0 so that x_NS diverses
		{
			printf("dFunc(%11.8lf) is zero!!", x_NS);
			break;
		}
		if (tol < _tol)									// When x_NS satisfies tolerance
		{
			printf("Solution is %11.8lf. \n", x_NS);
			return x_NS;
			break;
		}

		i += 1;
		x_c = x_NS;										// updating the value
	}
	if (i == (MaxIt + 1))								// When the solution is not found within the given number of iterationss
	{
		printf("Solution did not coverge within %d iterations at a required precision of %lf \n", MaxIt, _tol);
		return x_NS;
	}
}


/* Hybrid Method
	_a		: initial value #1 for bisection method
	_b		: initial value #2 for bisection method
	_x0		: Initial value for newton raphson method
	_tol	: tolerance	*/
double NR_hybrid(float _a, float _b, float _x0, float _tol)
{
	int MaxIt = 30, i = 0;					// Set the maximum iteration number
	float x_c = _x0, x_NS, tol;

	if (func(_a)*func(_b) > 0)				// Check whether the root is inside the interval
	{
		printf("True value(n_T) does not exist between 'a' and 'b' !!\n");
		printf("a= %f\t%f\t\tb = %f\t%f\n", _a, func(_a), _b, func(_b));
		return 0;
	}

	while (i <= MaxIt)						// Iteration 
	{
		if (dfunc(x_c) == 0)				// When dfunc = 0 so that x_NS diverses
		{
			printf("dFunc(%11.8lf) is zero!!\n", x_NS);
			break;
		}

		x_NS = x_c - (func(x_c) / dfunc(x_c));		// Calculating derivative and applying
		tol = (fabs(x_NS - x_c) / fabs(x_c));		// tolerance for hybrid Method
		printf("Iteration:%3d	x_NS: %11.8lf	tolerance:%11.8lf \n", i, x_NS, tol);

		if (x_NS > _b)								// When x_NS is in outside right of the interval
			x_NS = (x_NS - (_b - _a)) / 2;
		else if (x_NS < _a)							// When x_NS is in outside left of the interval
			x_NS = (x_NS + (_b - _a)) / 2;


		if (tol < _tol)								// When x_NS satisfies tolerance
		{
			printf("Solution is %11.8lf. \n", x_NS);
			return x_NS;
			break;
		}

		i += 1;
		x_c = x_NS;									// updating the value
	}
	if (i == (MaxIt + 1))							// When the solution is not found within the given number of iterationss
	{
		printf("Solution did not coverge within %d iterations at a required precision of %lf \n", MaxIt, _tol);
		return x_NS;
	}

}


/*--------------------------------------------------------------------------*/
/*							Linear Equation Solver 							*/
/*--------------------------------------------------------------------------*/


/* Gauss Elimination without Partial Pivoting
	_A		: input matrix _A
	_b		: input vector _b
	_U		: output matrix _U (upper triangular matrix)
	_bn		: output vector _bn */
void gaussElim(Matrix _A, Matrix _b, Matrix _U, Matrix _bn)
{
	double temp = 0;
	Matrix L = eye(_A.rows, _A.cols);						// Initialization of variables
	Matrix P = eye(_A.rows, _A.cols);						// P,L should be identity matrix for the calculation
	Matrix P_temp = copyMat(P);								// make temperary matrices for calculation and preserving original values
	Matrix m = createMat(_b.rows, 1);
	Matrix A_temp = copyMat(_A);
	Matrix _bn_temp = copyMat(_b);

	for (int k = 0; k < (A_temp.rows - 1); k++)
	{
		for (int i = k + 1; i < A_temp.rows; i++)
		{
			m.at[i][0] = (A_temp.at[i][k] / A_temp.at[k][k]);		// make m matrix for the efficient calculation
			if (A_temp.at[k][k] == 0)								// error case: divide by zero
			{
				printf("A[%d][%d] is zero that divides by zero!!\n", k, k);
				break;
			}
			else if (A_temp.rows != A_temp.cols)					// error case: not square matrix
			{
				printf("The matrix A is not square matrix!!\n");
				break;
			}
			else if (A_temp.rows != _b.rows)						// error case: A, b size disagreement
			{
				printf("The sizes of matrix A and vector b are not appropriate!!");
				break;
			}
			L.at[i][k] = m.at[i][0];
			for (int j = k; j < A_temp.cols; j++)					// gauss-elimination
			{
				A_temp.at[i][j] = A_temp.at[i][j] - m.at[i][0] * A_temp.at[k][j];
			}
			_bn_temp.at[i][0] = _bn_temp.at[i][0] - m.at[i][0] * _bn_temp.at[k][0];		// changing b is saved in bn
		}
	}
	
	for (int i = 0; i < _bn.rows; i++)
	{
		_bn.at[i][0] = _bn_temp.at[i][0];							// putting results into real matrix
	}

	for (int i = 0; i < _A.rows; i++)
	{
		
		for (int j = 0; j < _A.cols; j++)
		{
			_U.at[i][j] = A_temp.at[i][j];							// putting results into real matrix
		}
	}

	for (int i = 0; i < _U.rows; i++)								// Building permutation matrix
	{
		if (_U.at[i][i] < _U.at[i + 1][i + 1])						// compare the pivot values
		{
			for (int j = 0; j < _U.cols; j++)						// row exchange according to the pivot values
			{
				temp = P.at[i + 1][j];
				P.at[i + 1][j] = P.at[i][j];
				P.at[i][j] = temp;
			}
		}
		else break;
		
	}

	printf("\n[matrix P]\n\n");										// printing permutation matrix
	printMat(P);

	freeMat(L); freeMat(P); freeMat(P_temp); freeMat(m); freeMat(A_temp); freeMat(_bn_temp);
}

/* Back-substitution
	_U		: input matrix _U(upper triangular matrix)
	_bn		: input vector _bn
	_x		: output vector _x*/
void backsub(Matrix _U, Matrix _bn, Matrix _x)
{
	Matrix m = zeros(_x.rows, 1);
	Matrix _x_temp = copyMat(_x);									// make temperary matrices for calculation and preserving original values
	

	for (int i = _U.rows-1; i >= 0; i--)						
	{
		for (int j = i+1; j < _U.cols; j++)
		{
			 m.at[i][0] += _U.at[i][j] * _x_temp.at[j][0];			// pre-calculation of multiplication for efficiency
		}
		_x_temp.at[i][0] = (_bn.at[i][0] - m.at[i][0]) / _U.at[i][i];	// back-substitution
	}

	for (int i = 0; i < _x.rows; i++)
	{
		_x.at[i][0] = _x_temp.at[i][0];								// putting results into real matrix
	}

	freeMat(m); 
	freeMat(_x_temp);
}


/* Gauss Elimination with Scaled Partial Pivoting
	_A		: input matrix _A
	_b		: input vector _b
	_U		: output matrix _U (upper triangular matrix)
	_bn		: output vector _bn
	_P		: output matrix _P (permutation matrix)*/
void gaussElim(Matrix _A, Matrix _b, Matrix _U, Matrix _bn, Matrix _P)
{
	int max_idx;
	double temp = 0, max = 0;

	Matrix row_temp = zeros(1, _A.cols);
	Matrix P_temp = eye(_A.rows, _A.cols);							// make temperary matrices for calculation and preserving original values
	Matrix m = createMat(_b.rows, 1);
	Matrix A_temp = addColumn(_A, _b);
	Matrix bn_temp = copyMat(_b);

	for (int k = 0; k < (A_temp.rows - 1); k++)
	{
		// pivoting
		for (int j = k; j < _A.rows; j++)
		{
			max = A_temp.at[j][k];
			max_idx = j;
			for (int i = j + 1; i < _A.rows; i++)
			{
				if (A_temp.at[i][k] > max)
				{
					max = A_temp.at[i][k];
					max_idx = i;
				}
				for (int v = 0; v < P_temp.cols; v++)
				{
					row_temp.at[0][v] = P_temp.at[j][v];
					P_temp.at[j][v] = P_temp.at[max_idx][v];
					P_temp.at[max_idx][v] = row_temp.at[0][v];
				}

			}

			// switching
			for (int v = 0; v < A_temp.cols; v++)
			{
				row_temp.at[0][v] = A_temp.at[j][v];
				A_temp.at[j][v] = A_temp.at[max_idx][v];
				A_temp.at[max_idx][v] = row_temp.at[0][v];
			}
		}
		
		// elimination
		for (int i = k + 1; i < A_temp.rows; i++)
		{
			m.at[i][0] = (A_temp.at[i][k] / A_temp.at[k][k]);		// make m matrix for the efficient calculation
			
			if (_A.at[k][k] == 0)								// error case: divide by zero
			{
				printf("A[%d][%d] is zero that divides by zero!!\n", k, k);
				break;
			}
			else if (_A.rows != _A.cols)					// error case: not square matrix
			{
				printf("The matrix A is not square matrix!!\n");
				break;
			}
			else if (_A.rows != _b.rows)						// error case: A, b size disagreement
			{
				printf("The sizes of matrix A and vector b are not appropriate!!");
				break;
			}
			for (int j = k; j < A_temp.cols; j++)					// gauss-elimination
			{
				A_temp.at[i][j] = A_temp.at[i][j] - m.at[i][0] * A_temp.at[k][j];
			}
		}
	}

	for (int i = 0; i < _A.rows; i++)
	{
		bn_temp.at[i][0] = A_temp.at[i][_A.cols];
		_bn.at[i][0] = bn_temp.at[i][0];
	}

	for (int i = 0; i < _A.rows; i++)
	{
		for (int j = 0; j < _A.cols; j++)
		{
			_U.at[i][j] = A_temp.at[i][j];							// putting results into real matrix
			_P.at[i][j] = P_temp.at[i][j];
		}
	}

	/*
	freeMat(row_temp); 	
	freeMat(P_temp); 
	freeMat(A_temp);
	freeMat(bn_temp);
	freeMat(m);*/
}

/* LU Decomposition with Scaled Partial Pivoting
	_A		: input matrix _A
	_L		: output matrix _L (lower triangular matrix)
	_U		: output matrix _U (upper triangular matrix)
	_P		: output matrix _P (permutation matrix) */
void LUdecomp(Matrix _A, Matrix _L, Matrix _U, Matrix _P)
{
	int max_idx;
	double temp = 0, max = 0;

	Matrix row_temp = zeros(1, _A.cols);
	Matrix P_temp = eye(_A.rows, _A.cols);
	Matrix m = createMat(_A.rows, 1);
	Matrix A_temp = copyMat(_A);
	Matrix L_temp = eye(_L.rows, _L.cols);
	

	for (int k = 0; k < (A_temp.rows - 1); k++)
	{
		// pivoting
		for (int j = k; j < _A.rows; j++)
		{
			max = A_temp.at[j][k];
			max_idx = j;
			for (int i = j + 1; i < _A.rows; i++)
			{
				if (A_temp.at[i][k] > max)
				{
					max = A_temp.at[i][k];
					max_idx = i;
				}
				for (int v = 0; v < P_temp.cols; v++)
				{
					row_temp.at[0][v] = P_temp.at[j][v];
					P_temp.at[j][v] = P_temp.at[max_idx][v];
					P_temp.at[max_idx][v] = row_temp.at[0][v];
				}
			}
			// switching
			for (int v = 0; v < P_temp.cols; v++)
			{
				row_temp.at[0][v] = A_temp.at[j][v];
				A_temp.at[j][v] = A_temp.at[max_idx][v];
				A_temp.at[max_idx][v] = row_temp.at[0][v];
			}
		}

		// elimination
		for (int i = k + 1; i < A_temp.rows; i++)
		{
			m.at[i][0] = (A_temp.at[i][k] / A_temp.at[k][k]);		// make m matrix for the efficient calculation
			if (A_temp.at[k][k] == 0)								// error case: divide by zero
			{
				printf("A[%d][%d] is zero that divides by zero!!\n", k, k);
				break;
			}
			else if (A_temp.rows != A_temp.cols)					// error case: not square matrix
			{
				printf("The matrix A is not square matrix!!\n");
				break;
			}
			
			L_temp.at[i][k] = m.at[i][0];
			for (int j = k; j < A_temp.cols; j++)					// gauss-elimination
			{
				A_temp.at[i][j] = A_temp.at[i][j] - m.at[i][0] * A_temp.at[k][j];
			}
		}
	}

	for (int i = 0; i < _A.rows; i++)
	{
		for (int j = 0; j < _A.cols; j++)
		{
			_U.at[i][j] = A_temp.at[i][j];							// putting results into real matrix
			_P.at[i][j] = P_temp.at[i][j];
			_L.at[i][j] = L_temp.at[i][j];
		}
	}

	freeMat(row_temp); freeMat(P_temp); freeMat(A_temp); freeMat(L_temp); freeMat(m);
}


/* Forward substitution
	_L		: input matrix _L(lower triangular matrix)
	_bn		: input vector _bn
	_y		: output vector _y*/
void fwdsub(Matrix _L, Matrix _bn, Matrix _y)
{
	Matrix m = zeros(_L.rows, 1);
	Matrix _y_temp = copyMat(_y);									// make temperary matrices for calculation and preserving original values


	for (int i = 0; i < _y.rows; i++)
	{
		for (int j = i + 1; j < _L.cols; j++)
		{
			m.at[i][0] += _L.at[i][j] * _y_temp.at[j][0];			// pre-calculation of multiplication for efficiency
		}
		_y_temp.at[i][0] = (_bn.at[i][0] - m.at[i][0]) / _L.at[i][i];	// back-substitution
	}

	for (int i = 0; i < _y.rows; i++)
	{
		_y.at[i][0] = _y_temp.at[i][0];								// putting results into real matrix
	}

	freeMat(m);
	freeMat(_y_temp);
}


/* Solving linear system problem
	_A			: input matrix _A
	_b			: input vector _b
	_method		: input character _mehtod("g" or "l")
	_x			: output vector _x	*/
void solveLinear(Matrix _A, Matrix _b, const char* _method, Matrix _x)
{
	const char* ctl = _method;

	Matrix matU = createMat(_A.rows, _A.cols);
	Matrix matL = eye(_A.rows, _A.cols);
	Matrix matP = eye(_A.rows, _A.cols);
	Matrix matY = createMat(_A.rows, 1);
	Matrix vecbn = createMat(_A.rows, 1);
	Matrix _vecbn = createMat(_A.rows, 1);

	switch (*ctl)
	{
	case 'g' :
		gaussElim(_A, _b, matU, vecbn, matP);
		backsub(matU, vecbn, _x);
		break;

	case 'l':
		gaussElim(_A, _b, matU, vecbn, matP);
		LUdecomp(_A, matL, matU, matP);
		fwdsub(matL, vecbn, matY);				
		backsub(matU, matY, _x);
		break;

	default: printf("The methods are only 'g' or 'l' !!\n");
	}

	freeMat(matU);
	freeMat(matL);
	freeMat(matP);
	freeMat(matY);
	freeMat(vecbn);
	freeMat(_vecbn);
}

/*--------------------------------------------------------------------------*/
/*							Eigenvalues & Eigenvectors						*/
/*--------------------------------------------------------------------------*/


/* 2nd Norm: deriving second norm of the vector, size of the vector
	_A		: input matrix _A */
double norm2(Matrix _A)
{
	double result = 0;

	for (int i = 0; i < _A.rows; i++)
	{
		result += _A.at[i][0] * _A.at[i][0];	// Adding squares of the value in matrix _A
	}

	return sqrt(result);						// return square root of addition result
}


/* QR Decomposition revision
	_A		: input matrix _A
	_Q		: output matrix _Q (orthogonal matrix)
	_R		: output matrix _R (upper triangular matrix) */
void QRdecomp(Matrix _A, Matrix _Q, Matrix _R)
{
	double squared_norm = 0;
	Matrix I = eye(_A.rows, _A.rows);						// Preparing the matrices whhich will be used below
	Matrix c = zeros(_A.rows, 1);
	Matrix e = zeros(_A.rows, 1);
	Matrix v = zeros(_A.rows, 1);
	Matrix H = zeros(_A.rows, _A.cols);
	Matrix U_temp = zeros(_A.rows, _A.cols);
	Matrix Q_temp = copyMat(I);								// Initialize the Q as identity matrix
	Matrix R_temp = copyMat(_A);							// Initialize the R as  the same matrix with A

	for (int j = 0; j < _A.rows - 1; j++)
	{
		Matrix temp_vec = zeros(_A.rows, 1);				// initialize temperate matrices
		Matrix temp_mat1 = zeros(_A.rows, _A.cols);
		Matrix temp_mat2 = zeros(_A.rows, _A.cols);

		if (_A.rows != _A.cols)								// Check if input matrix is a square matrix!
		{
			printf("_A matrix is not a square matrix!!\n");
			break;
		}

		for (int i = 0; i < _A.rows; i++)					// allocating values in c & e to make Household Matrices
		{
			c.at[i][0] = R_temp.at[i][j];					
			for (int k = 0; k < j; k++)
			{
				c.at[k][0] = 0;
			}
			e.at[i][0] = 0;
			if (c.at[j][0] >= 0) e.at[j][0] = 1;
			else e.at[j][0] = -1;
		}

		for (int i = 0; i < _A.rows; i++)
		{
			temp_vec.at[i][0] = norm2(c) * e.at[i][0];		// norm(c) * e for making vector v
		}

		addMat(c, temp_vec, v);								// v = c + norm(c) * e
		multiMat(v, transpose(v), temp_mat1);				// v * v'
		squared_norm = norm2(v) * norm2(v);					// v' * v = norm(v)
		
		if (squared_norm == 0)							// Cheak whether the norm is zero, preventing division by zero!!
		{
			printf("Division by zero since the norm of v is zero!!\n");
			break;
		}
		for (int i = 0; i < _A.rows; i++)
		{
			for (int j = 0; j < _A.cols; j++)
			{
				temp_mat2.at[i][j] = 2 / squared_norm * temp_mat1.at[i][j];		// 2/(v' * v) * (v * v') => preparing household matrix
			}

		}

		subMat(I, temp_mat2, H);						// H = I - (2 * (v * v')) / (v' * v)
		Q_temp = multiMat(Q_temp, H);					// Q = Q * H  =>  Updating Q
		R_temp = multiMat(H, R_temp);					// R = H * R  =>  Updating R
	}

	for (int i = 0; i < _A.rows; i++)					// Assigning values in the parameter Matrices
	{
		for (int j = 0; j < _A.cols; j++)
		{
			_Q.at[i][j] = Q_temp.at[i][j];		
			_R.at[i][j] = R_temp.at[i][j];
		}
	}
}

/* Eigen values
	_A		: input matrix _A
	_Output : 3x1 output matrix(3 values) */
Matrix eig(Matrix _A)
{
	Matrix Q = zeros(_A.rows, _A.cols);
	Matrix R = zeros(_A.rows, _A.cols);
	Matrix U = copyMat(_A);								// initialize U as A
	
	Matrix eigval_vec = zeros(_A.rows, 1);

	for (int iter = 0; iter < 66; iter++)				// Iterate QRdecomp function until the U becomes an upper triangular matrix
	{
		QRdecomp(U, Q, R);
		U = multiMat(R, Q);
	}
	

	for (int i = 0; i < _A.rows; i++)					// Assigning values in the parameter Matrices
	{
		eigval_vec.at[i][0] = U.at[i][i];
	}
	
	return eigval_vec;
}


/* Eigen vecotors
	_A		: input matrix _A
	_Output : 3x3 output matrix(3 vectors) */
Matrix eigvec(Matrix _A)
{
	double* norm = new double[_A.rows]();					// Forming norm variable by Dynamic allocation
	Matrix eigval_vec = eig(_A);
	Matrix eigval_mat = zeros(_A.rows, _A.cols);
	Matrix eigvec_mat = zeros(_A.rows, _A.cols);
	Matrix* B = new Matrix[_A.rows]();						// Forming needed matrices by Dynamic allocation
	Matrix* minor_B = new Matrix[_A.rows]();				
	Matrix* inv_minor_B = new Matrix[_A.rows]();
	Matrix* eigvec_vec = new Matrix[_A.rows]();
	Matrix* temp_b = new Matrix[_A.rows]();
	for (int i = 0; i < _A.rows; i++)						// Assigning appropriate sizes to selected matrices
	{
		B[i] = zeros(_A.rows, _A.cols);
		minor_B[i] = zeros(_A.rows - 1, _A.cols - 1);
		inv_minor_B[i] = zeros(_A.rows - 1, _A.cols - 1);
		eigvec_vec[i] = zeros(_A.rows - 1, 1);
		temp_b[i] = zeros(_A.rows - 1, 1);
	}
	
	// Building lambda matrix
	for (int w = 0; w < _A.rows; w++)
	{
		for (int i = 0; i < _A.rows; i++)
		{
			B[w].at[i][i] = eigval_vec.at[w][0];				// Building lambda * I selectively
			for (int j = 0; j < _A.cols; j++)
			{
				B[w].at[i][j] = _A.at[i][j] - B[w].at[i][j];	// B = A - lambda * I
			}
		}
	}
						
	// Building sub_matrix
	for (int k = 0; k < _A.rows; k++) {
		int idxCol = 0, idxRow = 0;								// Declaring new index variables

		for (int i = 0; i < _A.rows; i++) {
			idxCol = 0;

			for (int j = 0; j < _A.cols; j++) {
				if (i != k && j != k) 
				{
					minor_B[k].at[idxRow][idxCol] = B[k].at[i][j];		// Building minor matrices of B
					idxCol++;
				}
				if (i != k && j == k)
				{
					temp_b[k].at[idxRow][0] = -1 * B[k].at[i][j];		// Extracting remaining values that will be used later
				}
			}
			if (k != i)
				idxRow++;
		}
		
		invMat(minor_B[k], inv_minor_B[k]);							// Generating inverse matrix of minor matrices
		multiMat(inv_minor_B[k], temp_b[k], eigvec_vec[k]);			// v = minor_B^(-1) * temp_b

		// Recovering minor matrices to 
		int col_idx = k, row_idx = 0;								// Declaring new index variables

		eigvec_mat.at[k][k] = 1;
		for (int j = 0; j < _A.cols-1; j++)
		{
			if (row_idx == col_idx)
			{
				row_idx++;
				j--;
				continue;
			}
			else
			{
				eigvec_mat.at[row_idx][col_idx] = eigvec_vec[k].at[j][0];		// Putting above results into the eigenvector matrix
				row_idx++;
			}
		}
		col_idx++;
	}


	// Normalizing the eigenvector matrices
	for (int i = 0; i < _A.rows; i++)
	{
		for (int j = 0; j < _A.cols; j++)
		{
			norm[i] += (eigvec_mat.at[j][i])*(eigvec_mat.at[j][i]);			// Extracting square term of eigenvectors
		}
	}

	for (int i = 0; i < _A.rows; i++)
	{
		for (int j = 0; j < _A.cols; j++)
		{
			eigvec_mat.at[i][j] /= sqrt(norm[j]);							// Dividing square rooted values of eigenvectors 
		}
	}
	
	free(norm);
	free(minor_B);
	free(inv_minor_B);
	free(eigvec_vec);
	free(temp_b);

	return eigvec_mat;
}



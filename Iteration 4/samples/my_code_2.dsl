/* This program is just a copy of sample_1.dsl.
You should write your second interesting FCAL
program and save it in a file with this name. */

main () {
	matrix newMatrix = matrix_read("./samples/my_code_2.data");
	matrix newMatrix2 = matrix_read("./samples/my_code_2_2.data");
	matrix newMatrix3 = newMatrix * newMatrix2;
	int rows;
	int iterator;
	int accumulator;
	rows = n_rows(newMatrix3);
	iterator = 0;
	accumulator = 0;
	while( iterator < rows )
		{accumulator = accumulator + newMatrix3[iterator:0];
		iterator = iterator + 1;}
	print( accumulator );
	print( "\n" );
	/* added all the values of the first col of
	a matrix */
}

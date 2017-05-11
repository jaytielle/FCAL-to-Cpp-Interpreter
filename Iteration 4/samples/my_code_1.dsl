/* This program is just a copy of sample_1.dsl. You should write your FCAL program to multiply two
2-dimensional matrices and save it
in a file with this name. */

main () {
	matrix newMatrix = matrix_read("./samples/my_code_1.data");
	matrix newMatrix2 = matrix_read("./samples/my_code_1_2.data");
	matrix newMatrix3 = newMatrix * newMatrix2;
	print( newMatrix3 );
}

main() {
  int num;
  num = 1;

  int sum_of_squares;
  sum_of_squares = 0;

  bool flag;
  flag = true;

  while( flag == true ) {
    sum_of_squares = sum_of_squares + num * num;
    num = num + 1;
    if ( num > max ) {
      flag = false;
    }
  }
print(sum_of_squares);
print("\n");
}
    

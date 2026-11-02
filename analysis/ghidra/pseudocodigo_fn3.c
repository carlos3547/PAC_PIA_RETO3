int compute(int a,int b)

{
  int b_local;
  int a_local;
  int i;
  int x;
  
  x = b + a * 7;
  for (i = 0; i < 3; i = i + 1) {
    x = x ^ a + i;
  }
  return x;
}

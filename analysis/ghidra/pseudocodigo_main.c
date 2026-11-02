int main(int argc,char **argv)
{
  bool bVar1;
  int iVar2;
  ostream *poVar3;
  void *this;
  char **argv_local;
  int argc_local;
  basic_string<> local_58;
  allocator<char> local_29;
  allocator<char> *local_28;
  int n;
  poVar3 = std::operator<<((ostream *)std::cout,"Hola, mundo!");
  std::ostream::operator<<(poVar3,std::endl<>);
  poVar3 = std::operator<<((ostream )std::cout,"MAGIC: ");
  poVar3 = std::operator<<(poVar3,MAGIC);
  std::ostream::operator<<(poVar3,std::endl<>);
  if (1 < argc) {
    local_28 = &local_29;
                    / try { // try from 00102313 to 00102317 has its CatchHandler @ 001023e7 /
    std::__cxx11::basic_string<>::basic_string<>(&local_58,argv[1],&local_29);
                    / try { // try from 00102329 to 0010232d has its CatchHandler @ 001023d6 */
    n = std::__cxx11::stoi(&local_58,(size_t *)0x0,10);
    std::__cxx11::basic_string<>::basic_string(&local_58);
    std::__new_allocator<char>::__new_allocator(&local_29.super___new_allocator<char>);
    bVar1 = validar(n);
    if (bVar1) {
      poVar3 = std::operator<<((ostream *)std::cout,"Resultado: ");
      iVar2 = compute(n,n * 2);
      this = (void *)std::ostream::operator<<(poVar3,iVar2);
      std::ostream::operator<<(this,std::endl<>);
    }
    else {
      poVar3 = std::operator<<((ostream *)std::cout,"Valor fuera de rango (0-100)");
      std::ostream::operator<<(poVar3,std::endl<>);
    }
  }
  return 0;
}

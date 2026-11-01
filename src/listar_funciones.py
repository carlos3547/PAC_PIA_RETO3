# listar_funciones.py — Script para Ghidra Script Manager
# Uso: Window > Script Manager > Ejecutar este script con el programa cargado
# Salida: imprime nombre y dirección de todas las funciones

from ghidra.program.model.listing import FunctionManager

def run():
    fm = currentProgram.getFunctionManager()
    funciones = list(fm.getFunctions(True))
    print("=" * 60)
    print(f"Total de funciones: {len(funciones)}")
    print("=" * 60)
    print(f"{'Nombre':<45} {'Dirección':>12}")
    print("-" * 60)
    for f in sorted(funciones, key=lambda x: x.getEntryPoint().getOffset()):
        nombre = f.getName()
        addr   = f.getEntryPoint().getOffset()
        print(f"{nombre:<45} 0x{addr:08X}")
    print("=" * 60)
    print("Copia esta salida a analysis/ghidra/funciones.txt")

run()

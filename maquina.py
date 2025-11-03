# Versão mais simples e direta
def carregar_programa_simples(nome_arquivo):
    """
    Versão mais simples que trata cada caso especificamente
    """
    programa = []
    
    with open(nome_arquivo, 'r') as arquivo:
        for linha in arquivo:
            linha = linha.strip()
            
            if not linha:
                continue
                
            partes = linha.split()
            rotulo, comando, valor1, valor2 = "", "", "", ""
            
            if not partes:
                continue
            
            # Caso 1: Linha com rótulo (ex: "L2 NULL")
            if len(partes) >= 2 and partes[0].startswith('L') and partes[0][1:].isdigit():
                rotulo = partes[0]
                comando = partes[1]
                # Processa valores se existirem
                if len(partes) > 2:
                    valores = ' '.join(partes[2:])
                    # Separa por vírgula se existir
                    if ',' in valores:
                        val_split = valores.split(',')
                        valor1 = val_split[0].strip()
                        valor2 = val_split[1].strip() if len(val_split) > 1 else ""
                    else:
                        valor1 = partes[2] if len(partes) > 2 else ""
                        valor2 = partes[3] if len(partes) > 3 else ""
            
            # Caso 2: Linha sem rótulo (ex: "ALLOC 0,2")
            else:
                comando = partes[0]
                if len(partes) > 1:
                    valores = ' '.join(partes[1:])
                    # Separa por vírgula se existir
                    if ',' in valores:
                        val_split = valores.split(',')
                        valor1 = val_split[0].strip()
                        valor2 = val_split[1].strip() if len(val_split) > 1 else ""
                    else:
                        valor1 = partes[1] if len(partes) > 1 else ""
                        valor2 = partes[2] if len(partes) > 2 else ""
            
            programa.append([rotulo, comando, valor1, valor2])
    
    return programa

def executar_programa_simples(programa,tamanho_memoria=100):
   
    # Mapeia rótulos para índices
    rotulos = {}
    for i, instrucao in enumerate(programa):
        rotulo = instrucao[0]
        if rotulo:
            rotulos[rotulo] = i
    
    i = 0  # Linha atual
    s = -1  # Posição de memoria
    memoria = [0] * tamanho_memoria
    
    print("=== EXECUÇÃO ATÉ ENCONTRAR HLT ===")
    
    while True:
        rotulo, comando, val1, val2 = programa[i]
        
        #(f"i={i}:{rotulo} {comando} {val1} {val2}")
        
        # PARA quando encontrar HLT
        if comando == "HLT":
            #(">>> ENCONTROU HLT - PARANDO EXECUÇÃO")
            break
        
        # Executa comandos de salto
        if comando == "LDC":
            #(">>> ENCONTROU LCD ")
            s +=1
            memoria[s] = int(val1)
            #(f"memoria = {memoria[s]}")
            i +=1
        
        elif comando == "LDV":
            #(">>> ENCONTROU LDV ")
            s +=1
            endereco = int(val1)
            memoria[s] = memoria[endereco]
            i +=1
        
        elif comando == "ADD":
            #(">>> ENCONTROU ADD ")
            memoria[s-1] = memoria[s-1] + memoria[s]
            s -=1
            i +=1

        elif comando == "SUB":
            #(">>> ENCONTROU SUB ")
            memoria[s-1] = memoria[s-1] - memoria[s]
            s -=1
            i +=1
        
        elif comando == "MULT":
            #(">>> ENCONTROU MULT ")
            memoria[s-1] = memoria[s-1] * memoria[s]
            s -=1
            i +=1
        
        elif comando == "DIVI":
            #(">>> ENCONTROU DIVI ")
            #(f"memS : {memoria[s]}     memS-1 : {memoria[s-1]}")
            valor1 = memoria[s-1]
            valor2 = memoria[s]
            memoria[s-1] = valor1 // valor2
            s -=1
            i +=1
        
        elif comando == "INV":
            #(">>> ENCONTROU INV ")
            memoria[s] = -memoria[s]
            i +=1

        elif comando == "AND":
            #(">>> ENCONTROU AND ")
            if memoria[s-1] == 1 and memoria[s] == 1: # memoria anterior deve ser igual a memoria atual ou os dois devem ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1
        elif comando == "OR":
            #(">>> ENCONTROU OR ")
            if memoria[s-1] == 1 or memoria[s] == 1: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1
        elif comando == "NEG":
            #(">>> ENCONTROU NEG ")
            memoria[s] = 1-memoria[s]
            i +=1

        elif comando == "CME":
            #(">>> ENCONTROU CME ")
            if memoria[s-1] < memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1
        elif comando == "CMA":
            #(">>> ENCONTROU CMA ")
            if memoria[s-1] > memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1

        elif comando == "CEQ":
            #(">>> ENCONTROU CEQ ")
            if memoria[s-1] == memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1

        elif comando == "CDIF":
            #(">>> ENCONTROU CDIF ")
            if memoria[s-1] != memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1

        elif comando == "CMEQ":
            #(">>> ENCONTROU CMEQ ")
            if memoria[s-1] <= memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1

        elif comando == "CMAQ":
            #(">>> ENCONTROU CMAQ ")
            if memoria[s-1] >= memoria[s]: # memoria anterior deve ser 1 OU a memoria atual deve ser 1 ? PERGUNTAR
                memoria[s-1] = 1
            else:
                memoria[s-1] = 0
            s -=1
            i +=1

        
        elif comando == "STR":
            #(">>> ENCONTROU STR ")
            memoria[int(val1)] = memoria[s]
            s -=1
            i +=1

        elif comando == "JMP":
            #(">>> ENCONTROU JMP ")
            i = rotulos[val1]

        elif comando == "JMPF":
            #(">>> ENCONTROU JMPF ")
            if memoria[s] == 0:
                i = rotulos[val1]
            else:
                i += 1
            s -=1
        
        elif comando == "RD":
            #(">>> ENCONTROU RD ")
            s +=1
            valor_lido = input("Entre com um valor inteiro: ")
            memoria[s] = int(valor_lido)
            i +=1

        elif comando == "PRN":
            #(">>> ENCONTROU PRN ")
            print(f"Saída: {memoria[s]}")
            input("Pressione Enter para continuar...")
            s-=1
            i +=1
        elif comando == "ALLOC":
            #(">>> ENCONTROU ALLOC ")
            m = int(val1)
            n = int(val2)
            for k in range(n):
                s +=1
                memoria[s] = memoria[m + k]
            i += 1
        
        elif comando == "DALLOC":
            #(">>> ENCONTROU DALLOC ")
            m = int(val1)
            n = int(val2)
            for  k in range(n-1, -1, -1):
                memoria[m + k] = memoria[s]
                s -=1
            i += 1
        
        elif comando == "CALL":
            #(">>> ENCONTROU CALL ")
            s +=1
            memoria[s] = i + 1  
            i = rotulos[val1]

        elif comando == "RETURN":
            #(">>> ENCONTROU RETURN ")
            i = memoria[s]  
            s -=1

        else:
            # Comandos normais
            #(">>> ENCONTROU NULL ")
            i += 1
        
        # Segurança contra loop infinito
        if i < 0 or i >= len(programa):
            #(">>> ERRO: i fora dos limites do programa")
            break

    # Mostra também uma visão geral da memória
    print("Memória completa (primeiras 20 posições):")
    for j in range(0, min(20, len(memoria))):
        marker = " <-- topo" if j == s else ""
        print(f"  [{j}]: {memoria[j]}{marker}")

    
    print("Fim da execução")
 
if __name__ == "__main__":
    nome_arquivo = "teste.txt"
    
    print("=== VERSÃO SIMPLES ===")
    programa = carregar_programa_simples(nome_arquivo)
    for i, instrucao in enumerate(programa):
        print(f"{i:2d}: {instrucao}")
    
    executar_programa_simples(programa)
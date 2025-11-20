class MaquinaVirtual:
    """
    Máquina Virtual com callbacks para integração com GUI
    """
    def __init__(self, tamanho_memoria=100):
        self.programa = []
        self.memoria = [0] * tamanho_memoria
        self.i = 0  # Program counter
        self.s = -1  # Stack pointer
        self.rotulos = {}
        self.executando = False
        self.pausado = False
        
        # Callbacks (a GUI pode registrar funções aqui)
        self.on_output = None  # Callback para PRN
        self.on_input_request = None  # Callback para RD (retorna valor)
        self.on_state_change = None  # Callback para atualização de estado
        self.on_halt = None  # Callback quando termina
        self.on_error = None  # Callback para erros
    
    def carregar_programa(self, nome_arquivo):
        """Carrega programa de arquivo"""
        self.programa = []
        
        with open(nome_arquivo, 'r', encoding='utf-8') as arquivo:
            for linha in arquivo:
                linha = linha.strip()
                if not linha:
                    continue
                
                partes = linha.split()
                rotulo, comando, valor1, valor2 = "", "", "", ""
                
                if not partes:
                    continue
                
                # Linha com rótulo
                if len(partes) >= 2 and partes[0].startswith('L') and partes[0][1:].isdigit():
                    rotulo = partes[0]
                    comando = partes[1]
                    if len(partes) > 2:
                        valores = ' '.join(partes[2:])
                        if ',' in valores:
                            val_split = valores.split(',')
                            valor1 = val_split[0].strip()
                            valor2 = val_split[1].strip() if len(val_split) > 1 else ""
                        else:
                            valor1 = partes[2] if len(partes) > 2 else ""
                            valor2 = partes[3] if len(partes) > 3 else ""
                else:
                    comando = partes[0]
                    if len(partes) > 1:
                        valores = ' '.join(partes[1:])
                        if ',' in valores:
                            val_split = valores.split(',')
                            valor1 = val_split[0].strip()
                            valor2 = val_split[1].strip() if len(val_split) > 1 else ""
                        else:
                            valor1 = partes[1] if len(partes) > 1 else ""
                            valor2 = partes[2] if len(partes) > 2 else ""
                
                self.programa.append([rotulo, comando, valor1, valor2])
        
        # Mapeia rótulos
        self.rotulos = {instr[0]: i for i, instr in enumerate(self.programa) if instr[0]}
        self.resetar()
        return len(self.programa)
    
    def resetar(self):
        """Reseta o estado da VM"""
        self.i = 0
        self.s = -1
        self.memoria = [0] * len(self.memoria)
        self.executando = False
        self.pausado = False
    
    def executar_instrucao(self):
        """Executa uma única instrução. Retorna True se deve continuar, False se parou"""
        if self.i >= len(self.programa):
            return False
        
        rotulo, comando, val1, val2 = self.programa[self.i]
        
        try:
            if comando == "HLT":
                self.executando = False
                if self.on_halt:
                    self.on_halt()
                return False
            
            elif comando == "START" or comando == "NULL":
                self.i += 1
            
            elif comando == "LDC":
                self.s += 1
                self.memoria[self.s] = int(val1)
                self.i += 1
            
            elif comando == "LDV":
                self.s += 1
                self.memoria[self.s] = self.memoria[int(val1)]
                self.i += 1
            
            elif comando == "ADD":
                self.memoria[self.s-1] = self.memoria[self.s-1] + self.memoria[self.s]
                self.s -= 1
                self.i += 1
            
            elif comando == "SUB":
                self.memoria[self.s-1] = self.memoria[self.s-1] - self.memoria[self.s]
                self.s -= 1
                self.i += 1
            
            elif comando == "MULT":
                self.memoria[self.s-1] = self.memoria[self.s-1] * self.memoria[self.s]
                self.s -= 1
                self.i += 1
            
            elif comando == "DIVI":
                valor1 = self.memoria[self.s-1]
                valor2 = self.memoria[self.s]
                self.memoria[self.s-1] = valor1 // valor2 if valor2 != 0 else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "INV":
                self.memoria[self.s] = -self.memoria[self.s]
                self.i += 1
            
            elif comando == "AND":
                self.memoria[self.s-1] = 1 if (self.memoria[self.s-1] == 1 and self.memoria[self.s] == 1) else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "OR":
                self.memoria[self.s-1] = 1 if (self.memoria[self.s-1] == 1 or self.memoria[self.s] == 1) else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "NEG":
                self.memoria[self.s] = 1 - self.memoria[self.s]
                self.i += 1
            
            elif comando == "CME":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] < self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "CMA":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] > self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "CEQ":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] == self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "CDIF":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] != self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "CMEQ":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] <= self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "CMAQ":
                self.memoria[self.s-1] = 1 if self.memoria[self.s-1] >= self.memoria[self.s] else 0
                self.s -= 1
                self.i += 1
            
            elif comando == "STR":
                self.memoria[int(val1)] = self.memoria[self.s]
                self.s -= 1
                self.i += 1
            
            elif comando == "JMP":
                self.i = self.rotulos[val1]
            
            elif comando == "JMPF":
                if self.memoria[self.s] == 0:
                    self.i = self.rotulos[val1]
                else:
                    self.i += 1
                self.s -= 1
            
            elif comando == "RD":
                # Solicita entrada via callback
                if self.on_input_request:
                    # A GUI pausará e chamará fornecer_entrada() depois
                    self.pausado = True
                    self.on_input_request()
                    return True  # Continua executando após entrada
                else:
                    # Modo console
                    valor = int(input("Entre com um valor inteiro: "))
                    self.s += 1
                    self.memoria[self.s] = valor
                    self.i += 1
            
            elif comando == "PRN":
                valor = self.memoria[self.s]
                if self.on_output:
                    self.on_output(valor)
                else:
                    print(f"Saída: {valor}")
                self.s -= 1
                self.i += 1
            
            elif comando == "ALLOC":
                m = int(val1)
                n = int(val2)
                for k in range(n):
                    self.s += 1
                    self.memoria[self.s] = self.memoria[m + k]
                self.i += 1
            
            elif comando == "DALLOC":
                m = int(val1)
                n = int(val2)
                for k in range(n-1, -1, -1):
                    self.memoria[m + k] = self.memoria[self.s]
                    self.s -= 1
                self.i += 1
            
            elif comando == "CALL":
                self.s += 1
                self.memoria[self.s] = self.i + 1
                self.i = self.rotulos[val1]
            
            elif comando == "RETURN":
                self.i = self.memoria[self.s]
                self.s -= 1
            
            else:
                self.i += 1
            
            # Notifica mudança de estado
            if self.on_state_change:
                self.on_state_change(self.i, self.s)
            
            return True
            
        except Exception as e:
            if self.on_error:
                self.on_error(str(e))
            self.executando = False
            return False
    
    def fornecer_entrada(self, valor):
        """Chamado pela GUI quando usuário fornece entrada para RD"""
        self.s += 1
        self.memoria[self.s] = int(valor)
        self.i += 1
        self.pausado = False
        
        # Notifica mudança de estado
        if self.on_state_change:
            self.on_state_change(self.i, self.s)
    
    def obter_estado(self):
        """Retorna estado atual da VM"""
        return {
            'i': self.i,
            's': self.s,
            'programa': self.programa,
            'memoria': self.memoria.copy(),
            'executando': self.executando,
            'pausado': self.pausado
        }


# Exemplo de uso standalone (modo console)
if __name__ == "__main__":
    vm = MaquinaVirtual()
    
    # Configura callbacks para console
    vm.on_output = lambda valor: print(f"📤 Saída: {valor}")
    vm.on_halt = lambda: print("🛑 Execução finalizada")
    vm.on_error = lambda msg: print(f"❌ Erro: {msg}")
    
    # Carrega e executa
    try:
        num_instrucoes = vm.carregar_programa("teste.txt")
        print(f"✅ Programa carregado: {num_instrucoes} instruções\n")
        
        vm.executando = True
        while vm.executando and vm.i < len(vm.programa):
            if not vm.executar_instrucao():
                break
        
        print("\n📊 Estado final:")
        print(f"   i={vm.i}, s={vm.s}")
        print(f"   Memória (primeiras 10): {vm.memoria[:10]}")
        
    except FileNotFoundError:
        print("❌ Arquivo 'teste.txt' não encontrado")
    except Exception as e:
        print(f"❌ Erro: {e}")
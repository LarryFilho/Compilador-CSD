import tkinter as tk
from tkinter import ttk, scrolledtext, filedialog, messagebox
import subprocess
import os
import threading
from maquina import MaquinaVirtual 

class ModernButton(tk.Canvas):
    def __init__(self, parent, text, command, color="#2196F3", **kwargs):
        super().__init__(parent, height=40, bg=parent['bg'], highlightthickness=0, **kwargs)
        self.command = command
        self._text = text
        self._color = color
        self.hover_color = self.adjust_color(color, 1.2)
        
        self.draw_button()
        self.bind("<Button-1>", lambda e: self.on_click())
        self.bind("<Enter>", lambda e: self.on_hover())
        self.bind("<Leave>", lambda e: self.on_leave())
    
    def update_button(self, text=None, color=None):
        if text is not None:
            self._text = text
        if color is not None:
            self._color = color
            self.hover_color = self.adjust_color(color, 1.2)
        self.draw_button()
    
    def adjust_color(self, color, factor):
        color = color.lstrip('#')
        r, g, b = tuple(int(color[i:i+2], 16) for i in (0, 2, 4))
        r = min(255, int(r * factor))
        g = min(255, int(g * factor))
        b = min(255, int(b * factor))
        return f'#{r:02x}{g:02x}{b:02x}'
    
    def draw_button(self, hover=False):
        self.delete("all")
        color = self.hover_color if hover else self._color
        width = self.winfo_reqwidth() or 120
        
        self.create_rounded_rect(2, 2, width-2, 38, radius=8, fill=color, outline="")
        self.create_text(width//2, 20, text=self._text, fill="white", 
                        font=('Segoe UI', 10, 'bold'))
    
    def create_rounded_rect(self, x1, y1, x2, y2, radius=25, **kwargs):
        points = [x1+radius, y1, x1+radius, y1, x2-radius, y1, x2-radius, y1, x2, y1,
                  x2, y1+radius, x2, y1+radius, x2, y2-radius, x2, y2-radius, x2, y2,
                  x2-radius, y2, x2-radius, y2, x1+radius, y2, x1+radius, y2, x1, y2,
                  x1, y2-radius, x1, y2-radius, x1, y1+radius, x1, y1+radius, x1, y1]
        return self.create_polygon(points, **kwargs, smooth=True)
    
    def on_hover(self):
        self.draw_button(hover=True)
    
    def on_leave(self):
        self.draw_button(hover=False)
    
    def on_click(self):
        self.command()


class CompiladorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Compilador CSD")
        self.root.geometry("1000x750")
        self.root.configure(bg='#f5f5f5')
        
        self.arquivo_atual = None
        self.criar_interface()
        self.mostrar_exemplo()
    
    def criar_interface(self):
        main_container = tk.Frame(self.root, bg='#f5f5f5')
        main_container.pack(fill='both', expand=True)
        
        header_frame = tk.Frame(main_container, bg='#667eea', height=80)
        header_frame.pack(fill='x', side='top')
        header_frame.pack_propagate(False)
        
        title_container = tk.Frame(header_frame, bg='#667eea')
        title_container.pack(side='left', padx=30, pady=20)
        
        tk.Label(
            title_container,
            text="🔧 Compilador CSD",
            font=('Segoe UI', 22, 'bold'),
            bg='#667eea',
            fg='white'
        ).pack(side='left')
        
        content_frame = tk.Frame(main_container, bg='#f5f5f5')
        content_frame.pack(fill='both', expand=True, padx=25, pady=20)
        
        toolbar_frame = tk.Frame(content_frame, bg='#ffffff', height=70)
        toolbar_frame.pack(fill='x', pady=(0, 15))
        toolbar_frame.pack_propagate(False)
        
        tk.Frame(content_frame, bg='#e5e7eb', height=2).place(
            in_=toolbar_frame, relx=0, rely=1, relwidth=1
        )
        
        toolbar_content = tk.Frame(toolbar_frame, bg='#ffffff')
        toolbar_content.pack(fill='both', expand=True, padx=20, pady=12)
        
        btn_frame = tk.Frame(toolbar_content, bg='#ffffff')
        btn_frame.pack(side='left')
        
        ModernButton(btn_frame, "📂 Abrir Arquivo", self.abrir_arquivo, "#3b82f6", width=140).pack(side='left', padx=5)
        ModernButton(btn_frame, "▶ Compilar & Executar", self.compilar, "#10b981", width=180).pack(side='left', padx=5)
        ModernButton(btn_frame, "🗑️ Limpar", self.limpar_tudo, "#6b7280", width=110).pack(side='left', padx=5)
        
        info_frame = tk.Frame(toolbar_content, bg='#f3f4f6', height=40)
        info_frame.pack(side='right', padx=5)
        
        self.label_arquivo = tk.Label(
            info_frame,
            text="💡 Use o exemplo ou carregue um arquivo",
            font=('Segoe UI', 9),
            bg='#f3f4f6',
            fg='#6b7280',
            padx=15,
            pady=8
        )
        self.label_arquivo.pack()
        
        editor_container = tk.Frame(content_frame, bg='#ffffff')
        editor_container.pack(fill='both', expand=True, pady=(0, 15))
        
        editor_header = tk.Frame(editor_container, bg='#ffffff', height=45)
        editor_header.pack(fill='x')
        editor_header.pack_propagate(False)
        
        tk.Label(
            editor_header,
            text="📝 Editor de Código Fonte",
            font=('Segoe UI', 12, 'bold'),
            bg='#ffffff',
            fg='#1f2937'
        ).pack(side='left', padx=20, pady=10)
        
        tk.Frame(editor_container, bg='#e5e7eb', height=1).pack(fill='x')
        
        editor_content = tk.Frame(editor_container, bg='#ffffff')
        editor_content.pack(fill='both', expand=True, padx=3, pady=3)
        
        self.text_editor = scrolledtext.ScrolledText(
            editor_content,
            wrap=tk.NONE,
            font=('JetBrains Mono', 11),
            bg='#fafafa',
            fg='#1f2937',
            insertbackground='#667eea',
            selectbackground='#667eea',
            selectforeground='white',
            padx=15,
            pady=15,
            border=0,
            relief='flat'
        )
        self.text_editor.pack(fill='both', expand=True)
        
        console_container = tk.Frame(content_frame, bg='#ffffff', height=200)
        console_container.pack(fill='x')
        console_container.pack_propagate(False)
        
        console_header = tk.Frame(console_container, bg='#1f2937', height=40)
        console_header.pack(fill='x')
        console_header.pack_propagate(False)
        
        tk.Label(
            console_header,
            text="💻 Console de Saída",
            font=('Segoe UI', 11, 'bold'),
            bg='#1f2937',
            fg='#ffffff'
        ).pack(side='left', padx=20, pady=8)
        
        console_content = tk.Frame(console_container, bg='#1f2937')
        console_content.pack(fill='both', expand=True)
        
        self.text_output = scrolledtext.ScrolledText(
            console_content,
            wrap=tk.WORD,
            font=('JetBrains Mono', 10),
            bg='#1f2937',
            fg='#e5e7eb',
            state='disabled',
            padx=15,
            pady=12,
            border=0,
            relief='flat'
        )
        self.text_output.pack(fill='both', expand=True)
        
        self.text_output.tag_config('error', foreground='#ef4444', font=('JetBrains Mono', 10, 'bold'))
        self.text_output.tag_config('success', foreground='#10b981', font=('JetBrains Mono', 10, 'bold'))
        self.text_output.tag_config('info', foreground='#3b82f6')
        self.text_output.tag_config('warning', foreground='#f59e0b')
        
        status_container = tk.Frame(self.root, bg='#ffffff', height=35)
        status_container.pack(fill='x', side='bottom')
        status_container.pack_propagate(False)
        
        tk.Frame(status_container, bg='#e5e7eb', height=1).pack(fill='x', side='top')
        
        self.status_label = tk.Label(
            status_container,
            text="● Pronto",
            font=('Segoe UI', 9),
            bg='#ffffff',
            fg='#6b7280',
            anchor='w'
        )
        self.status_label.pack(fill='both', padx=20, pady=8)
    
    def mostrar_exemplo(self):
        exemplo = """programa exemplo;
var
    numero, resultado: inteiro;
inicio
    leia(numero);
    resultado := numero * 2;
    escreva(resultado)
fim."""
        self.text_editor.insert(1.0, exemplo)
        self.adicionar_output("💡 Exemplo carregado. Modifique ou carregue seu próprio arquivo.\n", 'info')
        self.adicionar_output("▶️ Clique em 'Compilar & Executar' para começar!\n\n", 'success')
    
    def abrir_arquivo(self):
        filename = filedialog.askopenfilename(
            title="Abrir arquivo fonte",
            filetypes=[
                ("Arquivos de texto", "*.txt"),
                ("Arquivos Pascal", "*.pas"),
                ("Todos os arquivos", "*.*")
            ]
        )
        if filename:
            try:
                with open(filename, 'r', encoding='utf-8') as file:
                    conteudo = file.read()
                    self.text_editor.delete(1.0, tk.END)
                    self.text_editor.insert(1.0, conteudo)
                    self.arquivo_atual = filename
                    self.label_arquivo.config(text=f"📄 {os.path.basename(filename)}")
                    self.atualizar_status("✓ Arquivo carregado", 'success')
                    self.adicionar_output(f"\n📂 Arquivo carregado: {os.path.basename(filename)}\n", 'success')
            except Exception as e:
                messagebox.showerror("Erro", f"Erro ao abrir arquivo:\n{str(e)}")
    
    def compilar(self):
        conteudo = self.text_editor.get(1.0, tk.END).strip()
        if not conteudo:
            messagebox.showwarning("Aviso", "Nenhum código para compilar!")
            return
        
        with open('arquivo.txt', 'w', encoding='utf-8') as f:
            f.write(conteudo)
        
        self.atualizar_status("⏳ Compilando...", 'info')
        self.adicionar_output("\n" + "═" * 60 + "\n", 'info')
        self.adicionar_output("🔄 Iniciando compilação...\n", 'info')
        
        threading.Thread(target=self.executar_compilacao, daemon=True).start()
    
    def executar_compilacao(self):
        try:
            executavel = "Analisador.exe" if os.name == 'nt' else "Analisador"
            
            if not os.path.exists(executavel):
                self.adicionar_output("🔨 Compilando analisador...\n", 'info')
                
                if not os.path.exists("Analisador.c"):
                    self.adicionar_output("❌ Erro: Analisador.c não encontrado\n", 'error')
                    return
                
                compile_cmd = ["gcc", "Analisador.c", "-o", executavel]
                compile_result = subprocess.run(compile_cmd, capture_output=True, text=True)
                
                if compile_result.returncode != 0:
                    self.adicionar_output(f"❌ Erro ao compilar analisador:\n{compile_result.stderr}\n", 'error')
                    return
                
                self.adicionar_output("✅ Analisador compilado!\n", 'success')
            
            self.adicionar_output("🔍 Analisando código fonte...\n", 'info')
            
            if os.name == 'nt':
                result = subprocess.run([executavel], capture_output=True, timeout=30)
            else:
                result = subprocess.run(["./" + executavel], capture_output=True, timeout=30)
            
            try:
                output = result.stdout.decode('utf-8', errors='replace')
                stderr = result.stderr.decode('utf-8', errors='replace')
            except:
                output = result.stdout.decode('latin-1', errors='replace')
                stderr = result.stderr.decode('latin-1', errors='replace')
            
            output_completo = output + ("\n" + stderr if stderr else "")
            
            if "sucesso" in output_completo.lower() or result.returncode == 0:
                self.adicionar_output("✅ Compilação concluída com sucesso!\n\n", 'success')
                self.adicionar_output(output_completo + "\n", 'info')
                self.atualizar_status("● Compilação bem-sucedida!", 'success')
                
                arquivo_maquina = self.encontrar_arquivo_maquina()
                if arquivo_maquina:
                    self.adicionar_output(f"📦 Código de máquina: {arquivo_maquina}\n", 'success')
                    self.adicionar_output("🚀 Abrindo Máquina Virtual...\n", 'info')
                    self.root.after(500, lambda: self.abrir_vm(arquivo_maquina))
                else:
                    self.adicionar_output("⚠️ Aviso: Nenhum código de máquina foi gerado\n", 'warning')
            else:
                self.adicionar_output("❌ Erro na compilação:\n\n", 'error')
                self.adicionar_output(output_completo + "\n", 'warning')
                self.atualizar_status("● Erro na compilação", 'error')
                
        except subprocess.TimeoutExpired:
            self.adicionar_output("❌ Timeout: Compilação demorou muito\n", 'error')
        except FileNotFoundError:
            self.adicionar_output("❌ GCC não encontrado. Instale o compilador C\n", 'error')
        except Exception as e:
            self.adicionar_output(f"❌ Erro: {str(e)}\n", 'error')
        
        self.adicionar_output("═" * 60 + "\n", 'info')
    
    def encontrar_arquivo_maquina(self):
        prioridades = ['teste.txt', 'TESTE.TXT', 'saida.txt', 'output.txt']
        
        for arquivo in prioridades:
            if os.path.exists(arquivo):
                try:
                    with open(arquivo, 'r') as f:
                        conteudo = f.read()
                        if any(instr in conteudo for instr in ['LDC', 'LDV', 'ADD', 'JMP', 'HLT']):
                            return arquivo
                except:
                    continue
        
        return None
    
    def abrir_vm(self, arquivo):
        vm_window = tk.Toplevel(self.root)
        MaquinaVirtualGUI(vm_window, arquivo)
    
    def limpar_tudo(self):
        self.text_editor.delete(1.0, tk.END)
        self.text_output.config(state='normal')
        self.text_output.delete(1.0, tk.END)
        self.text_output.config(state='disabled')
        self.arquivo_atual = None
        self.label_arquivo.config(text="💡 Use o exemplo ou carregue um arquivo")
        self.atualizar_status("● Pronto", 'info')
        self.adicionar_output("✅ Tudo limpo! Digite seu código ou carregue um arquivo.\n", 'success')
    
    def adicionar_output(self, texto, tag='info'):
        self.text_output.config(state='normal')
        self.text_output.insert(tk.END, texto, tag)
        self.text_output.see(tk.END)
        self.text_output.config(state='disabled')
    
    def atualizar_status(self, texto, tipo='info'):
        cores = {'success': '#10b981', 'error': '#ef4444', 'info': '#3b82f6', 'warning': '#f59e0b'}
        self.status_label.config(text=texto, fg=cores.get(tipo, '#6b7280'))


class MaquinaVirtualGUI:
    """GUI que usa a MaquinaVirtual importada"""
    def __init__(self, root, arquivo_maquina=None):
        self.root = root
        self.root.title("Máquina Virtual CSD")
        self.root.geometry("1200x800")
        self.root.configure(bg='#f5f5f5')
        
        # Cria instância da VM
        self.vm = MaquinaVirtual()
        
        # Registra callbacks
        self.vm.on_output = self.handle_output
        self.vm.on_input_request = self.handle_input_request
        self.vm.on_state_change = self.handle_state_change
        self.vm.on_halt = self.handle_halt
        self.vm.on_error = self.handle_error
        
        self.criar_interface()
        
        if arquivo_maquina:
            self.root.after(100, lambda: self.carregar_automatico(arquivo_maquina))
    
    def criar_interface(self):
        header_frame = tk.Frame(self.root, bg='#764ba2', height=80)
        header_frame.pack(fill='x', side='top')
        header_frame.pack_propagate(False)
        
        title_container = tk.Frame(header_frame, bg='#764ba2')
        title_container.pack(side='left', padx=30, pady=20)
        
        tk.Label(
            title_container,
            text="⚙️ Máquina Virtual",
            font=('Segoe UI', 22, 'bold'),
            bg='#764ba2',
            fg='white'
        ).pack(side='left')
        
        content_frame = tk.Frame(self.root, bg='#f5f5f5')
        content_frame.pack(fill='both', expand=True, padx=25, pady=20)
        
        toolbar = tk.Frame(content_frame, bg='#ffffff', height=70)
        toolbar.pack(fill='x', pady=(0, 15))
        toolbar.pack_propagate(False)
        
        toolbar_content = tk.Frame(toolbar, bg='#ffffff')
        toolbar_content.pack(fill='both', expand=True, padx=20, pady=12)
        
        ModernButton(toolbar_content, "📂 Carregar", self.carregar_programa, "#3b82f6", width=120).pack(side='left', padx=3)
        ModernButton(toolbar_content, "▶ Executar", self.executar, "#10b981", width=120).pack(side='left', padx=3)
        self.btn_pausar = ModernButton(toolbar_content, "⏸ Pausar", self.pausar, "#f59e0b", width=120)
        self.btn_pausar.pack(side='left', padx=3)
        ModernButton(toolbar_content, "⏭ Passo", self.executar_passo, "#8b5cf6", width=120).pack(side='left', padx=3)
        ModernButton(toolbar_content, "🔄 Reset", self.resetar, "#6b7280", width=120).pack(side='left', padx=3)
        
        self.status_info = tk.Label(
            toolbar_content,
            text="i: 0  |  s: -1  |  Status: Aguardando",
            font=('JetBrains Mono', 10, 'bold'),
            bg='#f3f4f6',
            fg='#1f2937',
            padx=20,
            pady=10
        )
        self.status_info.pack(side='right')
        
        columns_frame = tk.Frame(content_frame, bg='#f5f5f5')
        columns_frame.pack(fill='both', expand=True)
        
        col1 = self.criar_card(columns_frame, "📋 Código de Máquina")
        col1.pack(side='left', fill='both', expand=True, padx=(0, 8))
        
        style = ttk.Style()
        style.configure("Treeview", font=('JetBrains Mono', 9), rowheight=28)
        style.configure("Treeview.Heading", font=('Segoe UI', 10, 'bold'))
        
        self.tree_codigo = ttk.Treeview(
            col1,
            columns=('Linha', 'Rótulo', 'Instrução', 'Atrib1', 'Atrib2'),
            show='headings',
            height=20
        )
        
        for col, width in [('Linha', 50), ('Rótulo', 70), ('Instrução', 100), ('Atrib1', 70), ('Atrib2', 70)]:
            self.tree_codigo.heading(col, text=col)
            self.tree_codigo.column(col, width=width)
        
        scrollbar1 = ttk.Scrollbar(col1, orient='vertical', command=self.tree_codigo.yview)
        self.tree_codigo.configure(yscrollcommand=scrollbar1.set)
        self.tree_codigo.pack(side='left', fill='both', expand=True, padx=10, pady=10)
        scrollbar1.pack(side='right', fill='y', padx=(0, 10), pady=10)
        
        col2 = self.criar_card(columns_frame, "💾 Memória (Pilha)")
        col2.pack(side='left', fill='both', expand=True, padx=8)
        
        self.tree_memoria = ttk.Treeview(
            col2,
            columns=('Endereço', 'Valor'),
            show='headings',
            height=20
        )
        
        self.tree_memoria.heading('Endereço', text='Endereço')
        self.tree_memoria.heading('Valor', text='Valor')
        self.tree_memoria.column('Endereço', width=100)
        self.tree_memoria.column('Valor', width=150)
        
        scrollbar2 = ttk.Scrollbar(col2, orient='vertical', command=self.tree_memoria.yview)
        self.tree_memoria.configure(yscrollcommand=scrollbar2.set)
        self.tree_memoria.pack(side='left', fill='both', expand=True, padx=10, pady=10)
        scrollbar2.pack(side='right', fill='y', padx=(0, 10), pady=10)
        
        col3 = self.criar_card(columns_frame, "💻 Console")
        col3.pack(side='left', fill='both', expand=True, padx=(8, 0))
        
        self.text_console = scrolledtext.ScrolledText(
            col3,
            wrap=tk.WORD,
            font=('JetBrains Mono', 10),
            bg='#1f2937',
            fg='#e5e7eb',
            state='disabled',
            padx=15,
            pady=12,
            border=0
        )
        self.text_console.pack(fill='both', expand=True, padx=10, pady=10)
        
        self.text_console.tag_config('success', foreground='#10b981')
        self.text_console.tag_config('error', foreground='#ef4444')
        self.text_console.tag_config('output', foreground='#3b82f6')
        self.text_console.tag_config('input', foreground='#f59e0b')
        
        self.tree_codigo.tag_configure('current', background='#667eea', foreground='white')
        self.tree_memoria.tag_configure('topo', background='#10b981', foreground='white')
        
        self.atualizar_visualizacao()
    
    def criar_card(self, parent, titulo):
        card = tk.Frame(parent, bg='#ffffff')
        header = tk.Frame(card, bg='#ffffff', height=45)
        header.pack(fill='x')
        header.pack_propagate(False)
        
        tk.Label(header, text=titulo, font=('Segoe UI', 11, 'bold'),
                bg='#ffffff', fg='#1f2937').pack(side='left', padx=15, pady=10)
        
        tk.Frame(card, bg='#e5e7eb', height=1).pack(fill='x')
        return card
    
    # ========== MÉTODOS QUE INTERAGEM COM A VM ==========
    
    def carregar_automatico(self, arquivo):
        try:
            num_instrucoes = self.vm.carregar_programa(arquivo)
            self.atualizar_visualizacao()
            self.adicionar_console(f"✅ Programa carregado: {os.path.basename(arquivo)}\n", 'success')
            self.adicionar_console(f"📋 {num_instrucoes} instruções\n", 'info')
            self.adicionar_console("🚀 Pronto! Clique em 'Executar'\n\n", 'success')
        except Exception as e:
            self.adicionar_console(f"❌ Erro ao carregar: {str(e)}\n", 'error')
    
    def carregar_programa(self):
        filename = filedialog.askopenfilename(
            title="Abrir código de máquina",
            filetypes=[("Arquivos de texto", "*.txt"), ("Todos", "*.*")]
        )
        if filename:
            self.carregar_automatico(filename)
    
    def executar(self):
        if not self.vm.programa:
            messagebox.showwarning("Aviso", "Carregue um programa primeiro!")
            return
        
        self.vm.executando = True
        self.vm.pausado = False
        self.iniciar_execucao()
    
    def iniciar_execucao(self):
        if not self.vm.executando or self.vm.pausado or self.vm.i >= len(self.vm.programa):
            return
        
        # Executa uma instrução
        continuar = self.vm.executar_instrucao()
        
        if not continuar or self.vm.i >= len(self.vm.programa):
            self.adicionar_console("\n✅ Execução concluída\n", 'success')
            self.vm.executando = False
            return
        
        # Agenda próxima instrução (se não pausou para entrada)
        if self.vm.executando and not self.vm.pausado:
            self.root.after(10, self.iniciar_execucao)
    
    def pausar(self):
        self.vm.pausado = not self.vm.pausado
        self.alternar_botao_pausa(self.vm.pausado)
        
        if self.vm.pausado:
            self.adicionar_console("⏸️ Execução pausada\n", 'warning')
        else:
            self.adicionar_console("▶️ Execução continuando\n", 'success')
            if self.vm.executando:
                self.iniciar_execucao()
    
    def alternar_botao_pausa(self, pausado):
        if pausado:
            self.btn_pausar.update_button(text="▶ Continuar", color="#10b981")
        else:
            self.btn_pausar.update_button(text="⏸ Pausar", color="#f59e0b")
    
    def executar_passo(self):
        if self.vm.programa and self.vm.i < len(self.vm.programa):
            self.vm.executar_instrucao()
            self.atualizar_visualizacao()
    
    def resetar(self):
        self.vm.resetar()
        self.atualizar_visualizacao()
        self.text_console.config(state='normal')
        self.text_console.delete(1.0, tk.END)
        self.text_console.config(state='disabled')
    
    # ========== CALLBACKS DA VM ==========
    
    def handle_output(self, valor):
        """Chamado quando a VM executa PRN"""
        self.adicionar_console(f"📤 Saída: {valor}\n", 'output')
    
    def handle_input_request(self):
        """Chamado quando a VM executa RD"""
        self.solicitar_entrada()
    
    def handle_state_change(self, i, s):
        """Chamado quando o estado da VM muda"""
        self.atualizar_visualizacao()
    
    def handle_halt(self):
        """Chamado quando a VM encontra HLT"""
        self.adicionar_console("\n🛑 Execução finalizada (HLT)\n", 'success')
    
    def handle_error(self, msg):
        """Chamado quando ocorre erro na VM"""
        self.adicionar_console(f"❌ Erro: {msg}\n", 'error')
    
    # ========== INTERFACE ==========
    
    def solicitar_entrada(self):
        """Abre dialog para entrada de dados (RD)"""
        dialog = tk.Toplevel(self.root)
        dialog.title("Entrada de Dados")
        dialog.geometry("400x180")
        dialog.configure(bg='#ffffff')
        dialog.transient(self.root)
        dialog.grab_set()
        dialog.resizable(False, False)
        
        dialog.update_idletasks()
        x = (dialog.winfo_screenwidth() // 2) - (dialog.winfo_width() // 2)
        y = (dialog.winfo_screenheight() // 2) - (dialog.winfo_height() // 2)
        dialog.geometry(f"+{x}+{y}")
        
        tk.Label(
            dialog,
            text="Digite um valor inteiro:",
            font=('Segoe UI', 12),
            bg='#ffffff',
            fg='#1f2937'
        ).pack(pady=(30, 15))
        
        entry = tk.Entry(
            dialog,
            font=('JetBrains Mono', 14),
            width=20,
            justify='center',
            bg='#f3f4f6',
            fg='#1f2937',
            relief='flat',
            bd=5
        )
        entry.pack(pady=10, padx=40)
        entry.focus()
        
        def confirmar():
            try:
                valor = int(entry.get())
                self.vm.fornecer_entrada(valor)
                self.adicionar_console(f"📥 Entrada: {valor}\n", 'input')
                dialog.destroy()
                self.atualizar_visualizacao()
                
                # Continua execução se estava executando
                if self.vm.executando and self.vm.i < len(self.vm.programa):
                    self.root.after(10, self.iniciar_execucao)
            except ValueError:
                messagebox.showerror("Erro", "Digite um número inteiro válido!", parent=dialog)
        
        btn_ok = ModernButton(dialog, "✓ Confirmar", confirmar, "#10b981", width=150)
        btn_ok.pack(pady=15)
        
        entry.bind('<Return>', lambda e: confirmar())
    
    def atualizar_visualizacao(self):
        """Atualiza a interface com o estado atual da VM"""
        estado = self.vm.obter_estado()
        i = estado['i']
        s = estado['s']
        programa = estado['programa']
        memoria = estado['memoria']
        executando = estado['executando']
        pausado = estado['pausado']
        
        # Atualiza código
        self.tree_codigo.delete(*self.tree_codigo.get_children())
        for idx, instr in enumerate(programa):
            tag = 'current' if idx == i else ''
            self.tree_codigo.insert('', 'end', values=(idx, *instr), tags=(tag,))
        
        if i < len(programa):
            items = self.tree_codigo.get_children()
            if i < len(items):
                self.tree_codigo.see(items[i])
        
        # Atualiza memória
        self.tree_memoria.delete(*self.tree_memoria.get_children())
        for idx in range(max(20, s + 5)):
            marker = " ← TOPO" if idx == s else ""
            tag = 'topo' if idx == s else ''
            self.tree_memoria.insert('', 'end', values=(f"[{idx}]", f"{memoria[idx]}{marker}"), tags=(tag,))
        
        if s >= 0:
            items = self.tree_memoria.get_children()
            if s < len(items):
                self.tree_memoria.see(items[s])
        
        # Atualiza status
        status_texto = "Executando ▶" if executando and not pausado else ("Pausado ⏸" if pausado else "Aguardando ●")
        self.status_info.config(text=f"i: {i}  |  s: {s}  |  Status: {status_texto}")
    
    def adicionar_console(self, texto, tipo='info'):
        """Adiciona texto ao console"""
        self.text_console.config(state='normal')
        self.text_console.insert(tk.END, texto, tipo)
        self.text_console.see(tk.END)
        self.text_console.config(state='disabled')


if __name__ == "__main__":
    root = tk.Tk()
    app = CompiladorGUI(root)
    root.mainloop()
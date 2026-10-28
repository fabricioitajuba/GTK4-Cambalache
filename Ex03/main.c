/****************************************************************************************
 * Ex03 - Salvando dados em um arquivo
 * 
 * Autor: Eng. Fabrício Ribeiro
 * para compilar: make
 * para limpar a compilação: make clean
 * para executar: ./Ex03
 * 
*****************************************************************************************/
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para passar as caixas de texto para a função do botão
typedef struct {
    GtkWidget *caixa_nome;
    GtkWidget *caixa_idade;
} WidgetsApp;

// Função auxiliar para descobrir o próximo ID disponível no arquivo
int obter_proximo_id() {
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        return 1; // Se o arquivo não existir, o primeiro ID será 1
    }

    int ultimo_id = 0;
    char linha[256];

    // Lê o arquivo linha por linha até o final para pegar o último ID registrado
    while (fgets(linha, sizeof(linha), arquivo)) {
        int id_atual;
        // Tenta ler o ID no início da linha (formato: "ID: X | ...")
        if (sscanf(linha, "ID: %d", &id_atual) == 1) {
            if (id_atual > ultimo_id) {
                ultimo_id = id_atual;
            }
        }
    }

    fclose(arquivo);
    return ultimo_id + 1; // Retorna o próximo ID disponível
}

// Função executada quando o botão "Salvar" for clicado
static void ao_pressionar_salvar(GtkButton *botao, gpointer user_data) {
    WidgetsApp *app_widgets = (WidgetsApp *)user_data;

    // Obtém os buffers das caixas de texto
    GtkEntryBuffer *buf_nome = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->caixa_nome));
    GtkEntryBuffer *buf_idade = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->caixa_idade));

    // Captura os textos digitados
    const char *nome = gtk_entry_buffer_get_text(buf_nome);
    const char *idade = gtk_entry_buffer_get_text(buf_idade);

    // Validação simples: não salva se os campos estiverem vazios
    if (strlen(nome) == 0 || strlen(idade) == 0) {
        g_print("Aviso: Preencha todos os campos antes de salvar.\n");
        return;
    }

    // Calcula o ID automático incremental
    int novo_id = obter_proximo_id();

    // Abre o arquivo de texto em modo "Append" (adicionar ao final)
    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo) {
        fprintf(arquivo, "ID: %d | Nome: %s | Idade: %s\n", novo_id, nome, idade);
        fclose(arquivo);
        g_print("Sucesso: Registro ID %d salvo com sucesso!\n", novo_id);

        // Limpa as caixas de texto na interface após salvar
        gtk_entry_buffer_set_text(buf_nome, "", -1);
        gtk_entry_buffer_set_text(buf_idade, "", -1);
    } else {
        g_printerr("Erro: Não foi possível abrir ou criar o arquivo 'usuarios.txt'.\n");
    }
}

// Função de ativação do aplicativo
static void ao_ativar_app(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *janela;
    GtkWidget *botao;
    
    static WidgetsApp app_widgets;

    // Carrega o arquivo XML da interface
    builder = gtk_builder_new_from_file("interface.ui");

    // Mapeia os componentes do XML para as variáveis em C
    janela = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    app_widgets.caixa_nome = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_nome"));
    app_widgets.caixa_idade = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_idade"));
    botao = GTK_WIDGET(gtk_builder_get_object(builder, "botao_salvar"));

    // Validação de segurança para garantir que nenhum ID falhou
    if (!janela || !app_widgets.caixa_nome || !app_widgets.caixa_idade || !botao) {
        g_printerr("Erro: Um ou mais IDs não foram encontrados no arquivo interface.ui\n");
        g_object_unref(builder);
        return;
    }

    gtk_application_add_window(app, GTK_WINDOW(janela));

    // Conecta o sinal de clique do botão à função de salvar
    g_signal_connect(botao, "clicked", G_CALLBACK(ao_pressionar_salvar), &app_widgets);

    // Apresenta a janela
    gtk_window_present(GTK_WINDOW(janela));

    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.CadastroTexto", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ao_ativar_app), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

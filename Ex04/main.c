/****************************************************************************************
 * Ex04 - Lendo dados em um arquivo
 * 
 * Autor: Eng. Fabrício Ribeiro
 * para compilar: make
 * para limpar a compilação: make clean
 * para executar: ./Ex04
 * 
*****************************************************************************************/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para mapear os widgets necessários na busca
typedef struct {
    GtkWidget *caixa_id;
    GtkWidget *label_nome;
    GtkWidget *label_idade;
} WidgetsBusca;

// Função que faz a varredura no arquivo de texto
static void ao_pressionar_buscar(GtkButton *botao, gpointer user_data) {
    WidgetsBusca *app_widgets = (WidgetsBusca *)user_data;

    // Captura o ID digitado pelo usuário
    GtkEntryBuffer *buf_id = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->caixa_id));
    const char *id_procurado_str = gtk_entry_buffer_get_text(buf_id);

    if (strlen(id_procurado_str) == 0) {
        gtk_label_set_text(GTK_LABEL(app_widgets->label_nome), "Nome: Digite um ID!");
        gtk_label_set_text(GTK_LABEL(app_widgets->label_idade), "Idade: —");
        return;
    }

    int id_procurado = atoi(id_procurado_str);

    // Abre o arquivo gerado pelo programa anterior para leitura
    FILE *arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        gtk_label_set_text(GTK_LABEL(app_widgets->label_nome), "Erro: Arquivo 'usuarios.txt' não encontrado.");
        gtk_label_set_text(GTK_LABEL(app_widgets->label_idade), "Cadastre alguém primeiro.");
        return;
    }

    char linha[256];
    gboolean encontrado = FALSE;

    // Varre o arquivo linha por linha buscando o formato "ID: X | Nome: Y | Idade: Z"
    while (fgets(linha, sizeof(linha), arquivo)) {
        int id_atual;
        char nome_atual[128];
        char idade_atual[32];

        // Processa a linha baseando-se estritamente na formatação que salvamos antes
        // O formato %[^\n|] lê tudo até encontrar uma barra vertical ou quebra de linha
        int correspondencias = sscanf(linha, "ID: %d | Nome: %[^|] | Idade: %[^\n]", 
                                      &id_atual, nome_atual, idade_atual);

        if (correspondencias == 3 && id_atual == id_procurado) {
            // Remove espaços em branco extras que possam sobrar nas pontas das strings
            g_strstrip(nome_atual);
            g_strstrip(idade_atual);

            // Cria os textos formatados para exibir nos labels
            char texto_nome[256];
            char texto_idade[64];
            g_snprintf(texto_nome, sizeof(texto_nome), "Nome: %s", nome_atual);
            g_snprintf(texto_idade, sizeof(texto_idade), "Idade: %s anos", idade_atual);

            // Atualiza a interface gráfica com os dados encontrados
            gtk_label_set_text(GTK_LABEL(app_widgets->label_nome), texto_nome);
            gtk_label_set_text(GTK_LABEL(app_widgets->label_idade), texto_idade);

            encontrado = TRUE;
            break; // Para a busca assim que encontrar
        }
    }

    fclose(arquivo);

    // Se varreu o arquivo todo e não achou o ID
    if (!encontrado) {
        gtk_label_set_text(GTK_LABEL(app_widgets->label_nome), "Status: Usuário não encontrado!");
        gtk_label_set_text(GTK_LABEL(app_widgets->label_idade), "Idade: —");
    }
}

// Inicialização da janela
static void ao_ativar_app(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *janela;
    GtkWidget *botao;
    
    static WidgetsBusca app_widgets;

    builder = gtk_builder_new_from_file("interface_busca.ui");

    janela = GTK_WIDGET(gtk_builder_get_object(builder, "window_busca"));
    app_widgets.caixa_id = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_id"));
    app_widgets.label_nome = GTK_WIDGET(gtk_builder_get_object(builder, "label_nome_resultado"));
    app_widgets.label_idade = GTK_WIDGET(gtk_builder_get_object(builder, "label_idade_resultado"));
    botao = GTK_WIDGET(gtk_builder_get_object(builder, "botao_buscar"));

    if (!janela || !app_widgets.caixa_id || !app_widgets.label_nome || !app_widgets.label_idade || !botao) {
        g_printerr("Erro: Verifique se todos os IDs estão corretos no XML de busca.\n");
        g_object_unref(builder);
        return;
    }

    gtk_application_add_window(app, GTK_WINDOW(janela));
    g_signal_connect(botao, "clicked", G_CALLBACK(ao_pressionar_buscar), &app_widgets);
    gtk_window_present(GTK_WINDOW(janela));

    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.exemplo.BuscarTexto", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ao_ativar_app), NULL);
    
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

/****************************************************************************************
 * Ex02 - Entrada e saída de dados
 * 
 * Autor: Eng. Fabrício Ribeiro
 * para compilar: make
 * para limpar a compilação: make clean
 * para executar: ./Ex02
 * 
*****************************************************************************************/
#include <gtk/gtk.h>

// Estrutura para passar as caixas de texto como argumento para o botão
typedef struct {
    GtkWidget *caixa1;
    GtkWidget *caixa2;
} WidgetsApp;

// Função chamada quando o botão é pressionado (Callback)
static void ao_pressionar_botao(GtkButton *botao, gpointer user_data) {
    WidgetsApp *app_widgets = (WidgetsApp *)user_data;

    // Em GTK4, caixas de texto usam um GtkEntryBuffer para gerenciar o texto
    GtkEntryBuffer *buffer1 = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->caixa1));
    GtkEntryBuffer *buffer2 = gtk_entry_get_buffer(GTK_ENTRY(app_widgets->caixa2));

    // Captura o texto digitado na primeira caixa
    const char *conteudo = gtk_entry_buffer_get_text(buffer1);

    // Copia o conteúdo capturado para a segunda caixa
    gtk_entry_buffer_set_text(buffer2, conteudo, -1);

    // Opcional: Limpa a primeira caixa de texto após transferir
    gtk_entry_buffer_set_text(buffer1, "", -1);
}

// Função de ativação do ciclo de vida do aplicativo
static void ao_ativar_app(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *janela;
    GtkWidget *botao;
    
    // Aloca memória estática/dinâmica para carregar as referências dos widgets
    static WidgetsApp app_widgets;

    // 1. Carrega o arquivo XML gerado pelo Cambalache
    builder = gtk_builder_new_from_file("interface.ui");

    // 2. Extrai os componentes da interface utilizando os IDs definidos
    janela = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    app_widgets.caixa1 = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_texto1"));
    app_widgets.caixa2 = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_texto2"));
    botao = GTK_WIDGET(gtk_builder_get_object(builder, "botao_transferir"));

    // Vincular a janela principal ao ciclo do aplicativo
    gtk_application_add_window(app, GTK_WINDOW(janela));

    // 3. Conecta o sinal de clique do botão à nossa função lógica, passando a struct
    g_signal_connect(botao, "clicked", G_CALLBACK(ao_pressionar_botao), &app_widgets);

    // Exibe a janela na tela
    gtk_window_present(GTK_WINDOW(janela));

    // Libera a memória do carregador do builder (os widgets já foram instanciados)
    g_object_unref(builder);
}

/*
static void ao_ativar_app(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *janela;
    GtkWidget *botao;
    
    static WidgetsApp app_widgets;

    // 1. Carrega o arquivo XML
    builder = gtk_builder_new_from_file("interface.ui");

    // 2. Extrai os componentes e verifica imediatamente se retornaram NULL
    janela = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    if (!janela) { g_printerr("Erro: ID 'main_window' não encontrado no XML!\n"); }

    app_widgets.caixa1 = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_texto1"));
    if (!app_widgets.caixa1) { g_printerr("Erro: ID 'caixa_texto1' não encontrado no XML!\n"); }

    app_widgets.caixa2 = GTK_WIDGET(gtk_builder_get_object(builder, "caixa_texto2"));
    if (!app_widgets.caixa2) { g_printerr("Erro: ID 'caixa_texto2' não encontrado no XML!\n"); }

    botao = GTK_WIDGET(gtk_builder_get_object(builder, "botao_transferir"));
    if (!botao) { g_printerr("Erro: ID 'botao_transferir' não encontrado no XML!\n"); }

    // Interrompe a execução antes de travar se algo crucial estiver faltando
    if (!janela || !botao) {
        g_printerr("Corrija os IDs no Cambalache ou no XML antes de continuar.\n");
        g_object_unref(builder);
        return;
    }

    // Vincular a janela ao aplicativo
    gtk_application_add_window(app, GTK_WINDOW(janela));

    // 3. Conecta o clique do botão
    g_signal_connect(botao, "clicked", G_CALLBACK(ao_pressionar_botao), &app_widgets);

    // Exibe a janela
    gtk_window_present(GTK_WINDOW(janela));

    g_object_unref(builder);
}
*/

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Inicializa a aplicação GTK
    app = gtk_application_new("org.exemplo.TransferirTextoC", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(ao_ativar_app), NULL);
    
    // Executa o loop principal
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

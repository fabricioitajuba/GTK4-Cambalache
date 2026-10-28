#include <gtk/gtk.h>

// Estrutura para passar múltiplos objetos para a função callback (se necessário)
// Neste caso, precisamos apenas da Label para alterar o texto.
typedef struct {
    GtkLabel *label;
} AppWidgets;

// Função chamada quando o botão é clicado
static void ao_clicar_no_botao(GtkButton *botao, gpointer user_data) {
    AppWidgets *widgets = (AppWidgets *)user_data;
    
    // Altera o texto da label de forma segura
    if (widgets->label != NULL) {
        gtk_label_set_text(widgets->label, "Hello World!");
    }
}

// Função de ativação do aplicativo
static void ao_ativar_app(GtkApplication *app, gpointer user_data) {
    GtkBuilder *builder;
    GtkWidget *window;
    GtkWidget *botao;
    
    // Aloca memória para a estrutura que guardará as referências dos nossos widgets
    AppWidgets *widgets = g_new0(AppWidgets, 1);

    // Inicializa o GtkBuilder e carrega o arquivo XML do Cambalache
    builder = gtk_builder_new_from_file("Ex01.ui");

    // 1. Obtém a janela principal e associa à aplicação
    window = GTK_WIDGET(gtk_builder_get_object(builder, "minha_janela"));
    gtk_window_set_application(GTK_WINDOW(window), app);

    // 2. Obtém os ponteiros da Label e do Botão criados no Cambalache usando os IDs
    widgets->label = GTK_LABEL(gtk_builder_get_object(builder, "minha_label"));
    botao = GTK_WIDGET(gtk_builder_get_object(builder, "meu_botao"));

    // 3. Conecta o sinal de clique de forma moderna (GTK 4)
    // Passamos a estrutura 'widgets' para que a função saiba qual label modificar
    if (botao != NULL) {
        g_signal_connect(botao, "clicked", G_CALLBACK(ao_clicar_no_botao), widgets);
    }

    // Libera a memória da estrutura de widgets automaticamente quando a janela for fechada
    g_object_set_data_full(G_OBJECT(window), "app-widgets", widgets, g_free);

    // Exibe a janela na tela
    gtk_window_present(GTK_WINDOW(window));

    // Como já pegamos as referências dos widgets, podemos liberar o builder da memória
    g_object_unref(builder);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Cria a aplicação GTK 4
    app = gtk_application_new("org.exemplo.HelloWorld", G_APPLICATION_DEFAULT_FLAGS);
    
    // Conecta o evento de inicialização
    g_signal_connect(app, "activate", G_CALLBACK(ao_ativar_app), NULL);
    
    // Executa e colhe o status de saída
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

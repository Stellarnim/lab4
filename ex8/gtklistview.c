#include <gtk/gtk.h>
/* 아래 enum 은 열을 구분하는 데 유용 */
enum
{
FIRST_COLUMN,
SECOND_COLUMN,
N_COLUMNS
};
void quit ( GtkWidget *window, gpointer data)
{
gtk_main_quit();
}
int main (int argc, char *argv[])
{
GtkWidget *window; 
GtkListStore *store;
GtkWidget *view;
GtkTreeIter iter;
GtkCellRenderer *renderer;

int i;

gtk_init(&argc, &argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "리스트");
gtk_window_set_default_size ( GTK_WINDOW(window), 300, 200);
g_signal_connect ( window, "destroy", G_CALLBACK(quit), NULL);
store = gtk_list_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_INT);

for (i = 0; i < 5; i++) {
gtk_list_store_append (store, &iter);
gtk_list_store_set (store, &iter, FIRST_COLUMN, "이 행의 번호는 ", SECOND_COLUMN, i, -1);
}

view = gtk_tree_view_new_with_model(GTK_TREE_MODEL (store));
renderer = gtk_cell_renderer_text_new();

gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 0, "제목", renderer, "title", FIRST_COLUMN, NULL);
gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), 1, "번호", renderer, "text", SECOND_COLUMN, NULL);

gtk_container_add(GTK_CONTAINER(window), view);
gtk_widget_show_all(window);
gtk_main ();

return 0;
}

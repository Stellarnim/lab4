#include <gtk/gtk.h>

/* 아래 enum 은 열을 구분하는 데 유용 */
enum {
COLUMN_TITLE, 
COLUMN_ARTIST, 
COLUMN_CATALOGUE, 
N_COLUMNS
};

void quit (GtkWidget *window, gpointer data)
{
gtk_main_quit();
}

int main (int argc, char *argv[])
{
GtkWidget *window; 
GtkTreeStore *store;
GtkWidget *view;
GtkTreeIter parent_iter, child_iter;
GtkCellRenderer *renderer;

gtk_init(&argc, &argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_title(GTK_WINDOW(window), "트리");
gtk_window_set_default_size (GTK_WINDOW(window), 300, 200); 
g_signal_connect (window, "destroy", G_CALLBACK(quit), NULL);

/* 트리 생성 및 열의 수와 타입 설정 */
store = gtk_tree_store_new (N_COLUMNS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

gtk_tree_store_append (store, &parent_iter, NULL); 
gtk_tree_store_set (store, &parent_iter, COLUMN_TITLE, "다시 부르기", COLUMN_ARTIST, "김광석", COLUMN_CATALOGUE, "A931001",-1);
gtk_tree_store_append (store, &child_iter, &parent_iter);
gtk_tree_store_set (store, &child_iter, COLUMN_TITLE, 0, "이등병의 편지", -1);

view = gtk_tree_view_new_with_model(GTK_TREE_MODEL (store));
renderer = gtk_cell_renderer_text_new ();

gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), COLUMN_TITLE, "제목", renderer, "text", COLUMN_TITLE, NULL);
gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), COLUMN_ARTIST, "아티스트", renderer, "text", COLUMN_ARTIST, NULL);
gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), COLUMN_CATALOGUE, "카탈로그", renderer, "text", COLUMN_CATALOGUE, NULL);

gtk_container_add(GTK_CONTAINER(window), view);
gtk_widget_show_all(window);
gtk_main ();

return 0;
}

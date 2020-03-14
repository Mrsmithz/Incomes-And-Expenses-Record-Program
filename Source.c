#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static GtkWidget *number1;
static GtkWidget *number2;
static GtkWidget *result;
static GtkWidget *listbox;

static GtkWidget * create_row_with_data(char *text) {
	GtkWidget *row, *label, *box;
	row = gtk_tree_view_column_new();
	box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	label = gtk_label_new(text);
	gtk_container_add(GTK_CONTAINER(row), box);
	gtk_container_add(GTK_CONTAINER(box), label);

	return row;
}
static GtkWidget * create_row() {
	GtkWidget *row, *label, *box;
	row = gtk_list_box_row_new();
	return row;
}

void do_calculate(GtkWidget *calculate, gpointer data) {
	int num1 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number1)));
	int num2 = atoi((char *)gtk_entry_get_text(GTK_ENTRY(number2)));

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "result: %d", num1 + num2);

	gtk_label_set_text(GTK_LABEL(result), buffer);
}

int main113(int argc, char**argv) {
	GtkWidget *window, *grid, *calculate, *layout, *label;
	GtkTreeViewColumn *col;
	
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(cssProvider),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

	layout = gtk_layout_new(NULL, NULL);
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), layout);

	listbox = gtk_tree_view_new();
	gtk_widget_set_size_request(GTK_TREE_VIEW(listbox), 500, 300);
	gtk_layout_put(GTK_LAYOUT(layout), GTK_TREE_VIEW(listbox), 10, 10);
	gtk_tree_view_insert_column(GTK_TREE_VIEW(listbox), create_row_with_data("ASDKJ"), -1);
	/*number1 = gtk_entry_new();
	gtk_widget_set_size_request(GTK_ENTRY(number1), 200, 100);
	gtk_layout_put(GTK_LAYOUT(layout), number1, 0, 100);

	number2 = gtk_entry_new();
	gtk_widget_set_size_request(GTK_ENTRY(number2), 200, 100);
	gtk_layout_put(GTK_LAYOUT(layout), number2, 220, 100);

	calculate = gtk_button_new_with_label("calculate");
	gtk_widget_set_size_request(GTK_BUTTON(calculate), 100, 100);
	gtk_layout_put(GTK_LAYOUT(layout), calculate, 430, 100);
	g_signal_connect(calculate, "clicked", G_CALLBACK(do_calculate), NULL);

	result = gtk_label_new("result:");
	gtk_widget_set_size_request(GTK_LABEL(calculate), 300, 100);	
	gtk_layout_put(GTK_LAYOUT(layout), result, 100, 300);*/
	
	gtk_widget_show_all(window);
	gtk_main();

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

GtkTreeStore *treestore;
GtkTreeIter toplevel, child;
GtkWidget *first, *last, *button, *calendar;
GtkWidget *window, *layout, *treeview;

enum
{
	dates = 0,
	notes,
	incomes,
	expenses,
	cols
};

static GtkTreeModel *create_model(void);
static GtkWidget *create_tree_view(void);
static void load_ui(void);
//static void add_data(void);

int main(int argc, char**argv) {
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	layout = gtk_layout_new(NULL, NULL);
	load_ui();
	gtk_container_add(GTK_WINDOW(window), layout);
	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}
static GtkTreeModel *create_model(void) {

	treestore = gtk_tree_store_new(cols, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

	/*gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, 0, "Faith", 1, "FAITH2", -1);

	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, 0, "Faith1", 1, "FAITH3", -1);*/

	return GTK_TREE_MODEL(treestore);
}
static GtkWidget *create_tree_view(void) {
	GtkTreeViewColumn *col1, *col2, *col3, *col4;
	GtkCellRenderer *cell1, *cell2, *cell3, *cell4;
	GtkWidget *treeview;
	GtkTreeModel *model;

	treeview = gtk_tree_view_new();

	col1 = gtk_tree_view_column_new();
	col2 = gtk_tree_view_column_new();
	col3 = gtk_tree_view_column_new();
	col4 = gtk_tree_view_column_new();
	
	gtk_tree_view_column_set_title(col1, "Dates");
	gtk_tree_view_column_set_title(col2, "Notes");
	gtk_tree_view_column_set_title(col3, "Incomes");
	gtk_tree_view_column_set_title(col4, "Expenses");
	
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col1);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col2);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col3);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col4);
	
	gtk_tree_view_column_set_fixed_width(col1, 100);
	gtk_tree_view_column_set_fixed_width(col2, 300);
	gtk_tree_view_column_set_fixed_width(col3, 190);
	gtk_tree_view_column_set_fixed_width(col4, 190);
	
	cell1 = gtk_cell_renderer_text_new();
	cell2 = gtk_cell_renderer_text_new();
	cell3 = gtk_cell_renderer_text_new();
	cell4 = gtk_cell_renderer_text_new();
	
	gtk_tree_view_column_pack_start(col1, cell1, TRUE);
	gtk_tree_view_column_pack_start(col2, cell2, TRUE);
	gtk_tree_view_column_pack_start(col3, cell3, TRUE);
	gtk_tree_view_column_pack_start(col4, cell4, TRUE);

	gtk_tree_view_column_add_attribute(col1, cell1, "text", dates);
	gtk_tree_view_column_add_attribute(col2, cell2, "text", notes);
	gtk_tree_view_column_add_attribute(col3, cell3, "text", incomes);
	gtk_tree_view_column_add_attribute(col4, cell4, "text", expenses);
	
	model = create_model();
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), model);
	g_object_unref(model);
	
	return treeview;
}
/*void add_data(void) {
	gtk_tree_store_append(treestore, &toplevel, NULL);
	gtk_tree_store_set(treestore, &toplevel, COL_FIRST_NAME, gtk_entry_get_text(GTK_ENTRY(first)), \
		COL_LAST_NAME, gtk_entry_get_text(GTK_ENTRY(last)), -1);
}*/
static void load_ui(void) {
	first = gtk_entry_new();
	last = gtk_entry_new();
	gtk_widget_set_size_request(GTK_ENTRY(first), 400, 50);
	gtk_widget_set_size_request(GTK_ENTRY(last), 400, 50);
	gtk_entry_set_placeholder_text(GTK_ENTRY(first), "First Name");
	gtk_entry_set_placeholder_text(GTK_ENTRY(last), "Last Name");
	//gtk_layout_put(GTK_LAYOUT(layout), first, 10, 350);
	//gtk_layout_put(GTK_LAYOUT(layout), last, 10, 400);

	button = gtk_button_new_with_label("Submit");
	gtk_widget_set_size_request(GTK_BUTTON(button), 400, 100);
	//gtk_layout_put(GTK_LAYOUT(layout), button, 10, 450);
	//g_signal_connect(button, "clicked", G_CALLBACK(add_data), NULL);
	calendar = gtk_calendar_new();
	gtk_widget_set_size_request(GTK_CALENDAR(calendar), 200, 100);
	gtk_layout_put(GTK_LAYOUT(layout), GTK_CALENDAR(calendar), 10, 315);

	treeview = create_tree_view();
	gtk_widget_set_size_request(GTK_TREE_VIEW(treeview), 770, 300);
	gtk_layout_put(GTK_LAYOUT(layout), treeview, 10, 10);
}
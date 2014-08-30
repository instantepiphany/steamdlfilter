#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <libconfig.h>




static void toggle_filter (GtkWidget *togglebutton,
                            gpointer   context_object)
{

  config_t *cfg = g_object_get_data (context_object, "cfg");
  const char * configpath = g_object_get_data (context_object, "configpath");
  config_setting_t *filter = g_object_get_data (context_object, "filter");
  GtkLabel * togglelb = g_object_get_data (context_object, "togglelb");

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(togglebutton))) {
    config_setting_set_bool (filter, 1);
    printf("Filter Enabled\n");
    gtk_label_set_text(togglelb, "Filter Enabled");
    }
  else {
    config_setting_set_bool (filter, 0);
    printf("Filter Disabled\n");
    gtk_label_set_text(togglelb, "Filter Disabled");
  }
  config_write_file(cfg, configpath);

}

static void toggle_logging (GtkWidget *togglebutton,
                            gpointer   context_object)
{

  config_t *cfg = g_object_get_data (context_object, "cfg");
  const char * configpath = g_object_get_data (context_object, "configpath");
  config_setting_t *logging = g_object_get_data (context_object, "logging");
  GtkLabel * logginglb = g_object_get_data (context_object, "logginglb");

  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(togglebutton))) {
    config_setting_set_bool (logging, 1);
    printf("Logging Enabled\n");
    gtk_label_set_text(logginglb, "Logging Enabled");
    }
  else {
    config_setting_set_bool (logging, 0);
    printf("Logging Disabled\n");
    gtk_label_set_text(logginglb, "Logging Disabled");
  }
  config_write_file(cfg, configpath);

}

static void save_server (GtkWidget *togglebutton,
                         gpointer   context_object)
{

  config_t *cfg = g_object_get_data (context_object, "cfg");
  const char * configpath = g_object_get_data (context_object, "configpath");
  GtkEntryBuffer *buffer = g_object_get_data (context_object, "buffer");

  const char * newserver = gtk_entry_buffer_get_text(buffer);
  config_setting_t * cfgsetting = config_lookup (cfg, "unmeteredserver");
  config_setting_set_string (cfgsetting, newserver);

  config_write_file(cfg, configpath);

}

int
main (int   argc,
      char *argv[])
{
  GtkWidget *window;
  GtkWidget *filtertglbtn;	
  GtkWidget *logtglbtn;
  GtkWidget *serverentry;
  GtkEntryBuffer *buffer;
  GtkWidget *grid;
  GtkLabel  *togglelb = GTK_LABEL(gtk_label_new("Filter Disabled"));
  GtkLabel  *logginglb = GTK_LABEL(gtk_label_new("Logging Disabled"));


  const char * chome = getenv("HOME");
  char precfg[100];
  char * config = strcpy(precfg, chome);
  const char * configpath = strcat(config,"/.config/steamdlfilter/config");
  const char * server;
  config_t cfg;
  config_setting_t *filter;
  config_setting_t *logging;
  config_init(&cfg);

  if(!config_read_file(&cfg, configpath))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),  config_error_line(&cfg), config_error_text(&cfg));
    printf("Error reading config file, if it exists at ~/.config/steamdlfilter/config, then the syntax may be incorrect.");
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  filter = config_lookup (&cfg, "filterenabled");
  if (config_setting_get_bool(filter) == 1)
    printf("Filter Enabled\n");
  else
    printf("Filter Disabled\n");

  logging = config_lookup (&cfg, "logging");
  if (config_setting_get_bool(logging) == 1)
    printf("Logging Enabled\n");
  else
    printf("Logging Disabled\n");

  config_lookup_string (&cfg, "unmeteredserver", &server);



  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (window), "steamdlfiltercfg");

  grid = gtk_grid_new ();
  
  g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);


  g_object_set_data (G_OBJECT(window), "cfg", &cfg); 
  g_object_set_data (G_OBJECT(window), "configpath", (gpointer) configpath);

  filtertglbtn = gtk_toggle_button_new();

  gtk_container_add(GTK_CONTAINER(filtertglbtn),GTK_WIDGET(togglelb));

  g_object_set_data (G_OBJECT(window), "filter", filter); 
  g_object_set_data (G_OBJECT(window), "togglelb", (gpointer) togglelb);
  
  gtk_grid_attach (GTK_GRID (grid), filtertglbtn, 0, 0, 1, 1);

  g_signal_connect (filtertglbtn, "toggled", G_CALLBACK(toggle_filter), window);
  
  if (config_setting_get_bool(filter) == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(filtertglbtn), TRUE);


  logtglbtn = gtk_toggle_button_new();

  gtk_container_add(GTK_CONTAINER(logtglbtn),GTK_WIDGET(logginglb));

  g_object_set_data (G_OBJECT(window), "logging", logging); 
  g_object_set_data (G_OBJECT(window), "logginglb", (gpointer) logginglb);

  gtk_grid_attach (GTK_GRID (grid), logtglbtn, 0, 1, 1, 2);

  g_signal_connect (logtglbtn, "toggled", G_CALLBACK(toggle_logging), window);

  if (config_setting_get_bool(logging) == 1)
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(logtglbtn), TRUE);


  serverentry = gtk_entry_new();
  buffer = gtk_entry_get_buffer(GTK_ENTRY(serverentry));
  gtk_entry_set_text(GTK_ENTRY(serverentry), server); 
  gtk_grid_attach (GTK_GRID (grid), serverentry, 0, 3, 1, 3);

  g_object_set_data (G_OBJECT(window), "buffer", buffer);

  g_signal_connect (serverentry, "activate", G_CALLBACK(save_server), window);



  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  gtk_container_add (GTK_CONTAINER (window), grid);


  gtk_widget_show_all (window);

  gtk_main ();
  config_write_file(&cfg, configpath);
  config_destroy(&cfg);
  return 0;
}



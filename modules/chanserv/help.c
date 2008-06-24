/*
 * Copyright (c) 2003-2004 E. Will et al.
 * Rights to this code are documented in doc/LICENSE.
 *
 * This file contains routines to handle the CService HELP command.
 *
 * $Id: help.c 8417 2007-06-08 00:48:04Z nenolod $
 */

#include "atheme.h"

DECLARE_MODULE_V1
(
	"chanserv/help", FALSE, _modinit, _moddeinit,
	"$Id: help.c 8417 2007-06-08 00:48:04Z nenolod $",
	"Atheme Development Group <http://www.atheme.org>"
);

static void cs_cmd_help(sourceinfo_t *si, int parc, char *parv[]);

command_t cs_help = { "HELP", N_("Displays contextual help information."),
                        AC_NONE, 1, cs_cmd_help };

list_t *cs_cmdtree, *cs_helptree;

void _modinit(module_t *m)
{
	MODULE_USE_SYMBOL(cs_cmdtree, "chanserv/main", "cs_cmdtree");
	MODULE_USE_SYMBOL(cs_helptree, "chanserv/main", "cs_helptree");

	command_add(&cs_help, cs_cmdtree);
	help_addentry(cs_helptree, "HELP", "help/help", NULL);
}

void _moddeinit()
{
	command_delete(&cs_help, cs_cmdtree);
	help_delentry(cs_helptree, "HELP");
}

/* HELP <command> [params] */
static void cs_cmd_help(sourceinfo_t *si, int parc, char *parv[])
{
	char *command = parv[0];

	/* strip off channel name for fantasy commands */
	if (si->c)
	{
		command = strchr(command, ' ');
		if (command != NULL)
			command++;
	}

	if (!command)
	{
		command_success_nodata(si, _("***** \2%s Help\2 *****"), chansvs.nick);
		command_success_nodata(si, _("\2%s\2 gives normal users the ability to maintain control\n"
					"of a channel, without the need of a bot. Channel takeovers are\n"
					"virtually impossible when a channel is registered with \2%s\2.\n"
					"Registration is a quick and painless process. Once registered,\n"
					"the founder can maintain complete and total control over the channel."),
				chansvs.nick, chansvs.nick);
		if (chansvs.expiry > 0)
		{
			command_success_nodata(si, _("Please note that channels will expire after %d days of inactivity,\n"
						"or if there are no eligible channel successors.\n"
						"Activity is defined as a user with one of %s being on the channel."), (chansvs.expiry / 86400), bitmask_to_flags2(CA_USEDUPDATE & ca_all, 0, chanacs_flags));
		}
		else
		{
			command_success_nodata(si, _("Please note that channels will expire if there are no eligible channel successors."));
		}
		command_success_nodata(si, _("Successors are primarily those who have the +R flag\n"
					"set on their account in the channel, although other\n"
					"people may be chosen depending on their access\n"
					"level and activity."));
		command_success_nodata(si, " ");
		if (chansvs.fantasy && config_options.join_chans && chansvs.trigger != '\0')
		{
			command_success_nodata(si, _("Commands can also be given on channel by prefixing one of '%s'\n"
						"and omitting the channel name. These are called \"fantasy\"\n"
						"commands and can also be disabled on a per-channel basis."), chansvs.trigger);
			command_success_nodata(si, " ");
		}
		command_success_nodata(si, _("For more information on a command, type:"));
		command_success_nodata(si, "\2/%s%s help <command>\2", (ircd->uses_rcommand == FALSE) ? "msg " : "", chansvs.disp);
		command_success_nodata(si, _("For a verbose listing of all commands, type:"));
		command_success_nodata(si, "\2/%s%s help commands\2", (ircd->uses_rcommand == FALSE) ? "msg " : "", chansvs.disp);
		command_success_nodata(si, " ");

		command_help_short(si, cs_cmdtree, "REGISTER OP INVITE UNBAN FLAGS RECOVER SET CLOSE FDROP FFLAGS FTRANSFER");

		command_success_nodata(si, _("***** \2End of Help\2 *****"));
		return;
	}

	if (!strcasecmp("COMMANDS", command))
	{
		command_success_nodata(si, _("***** \2%s Help\2 *****"), chansvs.nick);
		command_help(si, cs_cmdtree);
		command_success_nodata(si, _("***** \2End of Help\2 *****"));
		return;
	}

	help_display(si, command, cs_helptree);
}

/* vim:cinoptions=>s,e0,n0,f0,{0,}0,^0,=s,ps,t0,c3,+s,(2s,us,)20,*30,gs,hs
 * vim:ts=8
 * vim:sw=8
 * vim:noexpandtab
 */

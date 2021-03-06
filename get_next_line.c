/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rjeraldi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 11:41:47 by rjeraldi          #+#    #+#             */
/*   Updated: 2020/11/08 02:16:42 by rjeraldi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int					bufparse(char **buf, char **line)
{
	char			*eol;
	char			*strtmp;
	size_t			len;

	if (!(eol = NULL) && (!(*buf) || **buf == '\0'))
		return (-1);
	if ((eol = ft_strchr(*buf, '\n')))
	{
		*line = ft_strsub(*buf, 0, eol - *buf);
		len = ft_strlen(*line);
		if (ft_strlen(*buf) > len)
		{
			strtmp = *buf;
			*buf = ft_strsub(strtmp, len + 1, ft_strlen(strtmp) - len - 1);
			ft_strdel(&strtmp);
			return (1);
		}
		else
		{
			ft_strdel(buf);
			return (0);
		}
	}
	return (-1);
}

void				buftotmp(char **buf, char **strtmp, ssize_t *buflen)
{
	if (*buf == NULL)
	{
		*buf = ft_strnew(0);
		*buflen = 0;
	}
	else
		*buflen = ft_strlen(*buf);
	*strtmp = ft_strnew(*buflen + BUFF_SIZE);
	ft_strlcat(*strtmp, *buf, *buflen + 1);
	ft_strdel(buf);
}

int					get_next_line(const int fd, char **line)
{
	static char		*buf[FD_MAX];
	int				out;
	ssize_t			ssread;
	ssize_t			buflen;
	char			*strtmp;

	if (fd < 0 || fd > FD_MAX - 1 || line == NULL)
		return (-1);
	while ((out = bufparse(&buf[fd], line)) == -1)
	{
		buftotmp(&buf[fd], &strtmp, &buflen);
		ssread = read(fd, (strtmp + buflen), BUFF_SIZE);
		buf[fd] = strtmp;
		if (ssread == 0)
		{
			*line = buf[fd];
			buf[fd] = NULL;
			if (!(out = ft_strlen(*line) > 0 ? 1 : 0))
				ft_strdel(line);
			break ;
		}
		else if (ssread == -1)
			return (-1);
	}
	return (out);
}

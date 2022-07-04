/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ksagon <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 11:45:25 by ksagon            #+#    #+#             */
/*   Updated: 2021/11/16 14:35:09 by ksagon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	char		*p1;
	const char	*p2;
	size_t		i;
	size_t		dlen;

	i = size;
	p1 = dst;
	while (*p1 != '\0' && i-- != 0)
		p1++;
	dlen = p1 - dst;
	i = size - dlen;
	p2 = src;
	if (!i)
		return (dlen + ft_strlen(p2));
	while (*p2 != '\0')
	{
		if (i != 1)
		{
			*p1++ = *p2;
			i--;
		}
		p2++;
	}
	*p1 = '\0';
	return (dlen + (p2 - src));
}

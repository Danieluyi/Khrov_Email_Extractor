/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_email.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/24 22:08:11 by euyi              #+#    #+#             */
/*   Updated: 2022/01/16 06:16:53 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

// Comments Coming soon

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[(i)])
		i++;
	return (i);
}

void	remove_at(char *ptr)
{
	int i;
	
	i = 0;
	while (ptr[i])
	{
		i++;
		if (ptr[i] == '@')
			ptr[i] = 1;
	}
	if (ptr[i] == 0 && i < 35)
		ptr[i] = 1;
}

char	*shift_back(char *ptr)
{
	int			i;

	i = 0;
	while (i < 35 && ptr[i])
	{
		ptr[i] = ptr[i +1];
		i++;
	}
	ptr[i] = 0;
	return (ptr);	
}

int email_char_check(char c)
{
	char	*ptr;
	int		i;

	ptr = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.";
	i = 0;
	while (ptr[i])
	{
		if (ptr[i] == c)
			return (1);
		i++;	
	}
	return (0);
}

char	*one_email(char *src, char **str)
{
	int	i;
	int	j;
	int marker;
	
	i = 0;
	j = 0;
	while (src[i] != '@')
		i++;
	while ((!email_char_check(src[i]) || src[i] == '.') && i > 0)
		i--;
	marker = i;
	while (email_char_check(src[i]) && !(src[i] == '.' && src[i - 1] == '.') && i > 0)
		i--;
	if (i != 0)
		i++;
	while (src[i] == '.')
		i++;
	if (i == marker && !email_char_check(src[i])) 
		return (NULL);
	while (email_char_check(src[i]))
	{
		src[j] = src[i];
		j++;
		i++;
	}
	while (src[j - 1] == '.')
		j--;
	while (src[i] != '@')
		i++;
	src[i] = 1;
	src[j] = '@';
	j++;
	while (!email_char_check(src[i]))
		i++;
	while (email_char_check(src[i]) && src[i] != '.')
	{
		src[j] = src[i];
		j++;
		i++;
	}
	while (**str)
	{
		src[j] = **str;
		j++;
		(*str)++;
	}
	src[j] = '\0';
	return (src);
}

char	*check_for_com(char *s)
{
	char	*ptr;
	int		i;
	int		j;
	
	i = 0;
	j = 0;
	if (!s || (s && !s[0]))
		return (NULL);
	while (!email_char_check(s[i]) && s[0])
		s++;
	while (email_char_check(s[i]))
		i++;
	if (s[i] == 0 || i < 2)
		return (NULL);
	ptr = (char *) malloc(sizeof(char) * 30);
	ptr[j] = '.';
	while (j < i)
	{
		ptr[j + 1] = s[j];
		j++;
	}
	if (ptr[j + 1] != 0)
		ptr[j + 1] = 0;
	return (ptr);
}

char	*check_for_dot(char *src)
{
	int	i;

	i = 0;
	if (src[i] == '.' || !src[i])
		return (NULL);
	while (src[i])
	{
		if (src[i] == '.' && src[i + 1])
			return (src + (i + 1));
		else if (!strncasecmp(src + i, "[dot]", 5) || 
			 	 !strncasecmp(src + i, "(dot)", 5) || 
				 !strncasecmp(src + i, "*dot*", 5) )
			{
				memcpy(src + i, ".\1\1\1\1", 5);
				return (src + (i + 5));
			}
		i++;
	}
	return (NULL);
}

// This checks for the occurence of str (@ cloaks) in string src.
// Param 3 is the strlen of str that we're checking for in source.
// If finds str in src, it replaces all occurence of str with ASCII
// 1, then it replaces the firt byte with'@' and 
// returns a dest that points to the array immediately after the
// array element where it has inputed the '@' sign. 
// If str doesn't occur in src, or if str 
// occurs in the very first byte of src, it will return NULL.

char	*check_for_at(char *src)
{
	int	i;

	i = 0;
	if (src[i] == '@' || !src[i])
		return (NULL);
	while (src[i])
	{
		if (src[i] == '@' && src[i + 1])
			return (src + (i + 1));
		else if (!strncasecmp(src + i, "[at]", 4) || 
				 !strncasecmp(src + i, "(at)", 4) || 
				 !strncasecmp(src + i, "*at*", 4) )
			{
				memcpy(src + i, "@\1\1\1", 4);
				return (src + (i + 4));
			}
		i++;
	}
	return (NULL);
}

 // This function is always guaranteed to receive a strlen of 35
 // Then it proceeds to check if there is an email
 // If, yes, it prints out the email to output, then it APPENDS ",\n" to output
 // Then it sets ptr from arr[0] to last email array used to '`'
 // then it returns ptr
 // If no email found it returns NULL
int	email_detect(char *ptr)
{
	char	*string;
	
	string = check_for_at(ptr);

	if (!string)
		return (0);
	string = check_for_dot(string);
	if (!string)
		return (0);
	string = check_for_com(string);
	if (!string)
		return (0);
	if (one_email(ptr, &string))
		return ((int)ft_strlen(ptr));
	return (0);
}

int	get_next_email(int output, int input) // output emails to a new output and return number of emails read
{
	char	arr[36];
	char	*ptr;
	int		i;
	int		j;
	int		counter;

	if (write(output, "#################### KHROV EMAIL EXTRACTOR ####################\n\n", 65)
		< 65 && write(1, "Error Writing to Output.txt", 27))
		return (-1);
	i = 0;
	ptr = arr;
	j = read(input, ptr, 35);
	if ((j == -1 && write(1, "Error reading from input\n", 25) > 0) ||
		(j == 0 && write(1, "No Text to read from Input\n", 27) > 0) ||
		(j < 35 && write(1, "Text in input too short\n", 24) > 0))
		return (1);
	ptr[35] = 0;
	counter = 0;
	while (j != 0 && j != -1)
	{
		i = email_detect(ptr);
		if (i > 0 && ++counter)
		{
			write(output, ptr, i);
			write(output, ", \n", 3);
			remove_at(ptr);
		}
		shift_back(ptr);
		j = read(input, &ptr[34], 1);
	}
	return (counter);
}

#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int input;
	int output;
	int i;
	char a;
	
	write(1, "\tPaste The raw emails inside input.txt, then save and close the text file\n\tAfter saving the input.txt, press y and hit enter below: \n\tOtherwise, press any other key to exit program :\n\t", 184);
	read(0, &a, 1);
	if (a != 'y')
		return (0);		
	input = open("input.txt", O_RDONLY);
	output = open("output.txt", O_WRONLY | O_APPEND);
	i = get_next_email(output, input);
	if (i > 0)
		write(1, "\tSuccess!\n\t15 Email Addresses Saved to Output.txt\n", 50);
	sleep(5);
	return (0);
}

// we will assign report to -1, 0, and 1 return values of get_next_email()

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcutte <rcutte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 12:11:28 by abourgeo          #+#    #+#             */
/*   Updated: 2024/02/09 15:11:04 by rcutte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

int	main(void)
{
	char* input;
	t_token *tokens;

    while(1) {
        input = readline(">> ");

        if (!input) {
            break;
        }

        add_history(input);
        printf("You entered: %s\n", input);
		tokens = lexer(input);
		print_tokens(tokens);
        free(input);
    }
    return 0;
}
